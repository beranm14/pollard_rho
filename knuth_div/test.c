 
01509 static void KnuthDiv(unsigned *u, unsigned *v, unsigned *q, unsigned* r,
01510                      unsigned m, unsigned n) {
01511   assert(u && "Must provide dividend");
01512   assert(v && "Must provide divisor");
01513   assert(q && "Must provide quotient");
01514   assert(u != v && u != q && v != q && "Must use different memory");
01515   assert(n>1 && "n must be > 1");
01516 
01517   // b denotes the base of the number system. In our case b is 2^32.
01518   LLVM_CONSTEXPR uint64_t b = uint64_t(1) << 32;
01519 
01520   DEBUG(dbgs() << "KnuthDiv: m=" << m << " n=" << n << '\n');
01521   DEBUG(dbgs() << "KnuthDiv: original:");
01522   DEBUG(for (int i = m+n; i >=0; i--) dbgs() << " " << u[i]);
01523   DEBUG(dbgs() << " by");
01524   DEBUG(for (int i = n; i >0; i--) dbgs() << " " << v[i-1]);
01525   DEBUG(dbgs() << '\n');
01526   // D1. [Normalize.] Set d = b / (v[n-1] + 1) and multiply all the digits of
01527   // u and v by d. Note that we have taken Knuth's advice here to use a power
01528   // of 2 value for d such that d * v[n-1] >= b/2 (b is the base). A power of
01529   // 2 allows us to shift instead of multiply and it is easy to determine the
01530   // shift amount from the leading zeros.  We are basically normalizing the u
01531   // and v so that its high bits are shifted to the top of v's range without
01532   // overflow. Note that this can require an extra word in u so that u must
01533   // be of length m+n+1.
01534   unsigned shift = countLeadingZeros(v[n-1]);
01535   unsigned v_carry = 0;
01536   unsigned u_carry = 0;
01537   if (shift) {
01538     for (unsigned i = 0; i < m+n; ++i) {
01539       unsigned u_tmp = u[i] >> (32 - shift);
01540       u[i] = (u[i] << shift) | u_carry;
01541       u_carry = u_tmp;
01542     }
01543     for (unsigned i = 0; i < n; ++i) {
01544       unsigned v_tmp = v[i] >> (32 - shift);
01545       v[i] = (v[i] << shift) | v_carry;
01546       v_carry = v_tmp;
01547     }
01548   }
01549   u[m+n] = u_carry;
01550 
01551   DEBUG(dbgs() << "KnuthDiv:   normal:");
01552   DEBUG(for (int i = m+n; i >=0; i--) dbgs() << " " << u[i]);
01553   DEBUG(dbgs() << " by");
01554   DEBUG(for (int i = n; i >0; i--) dbgs() << " " << v[i-1]);
01555   DEBUG(dbgs() << '\n');
01556 
01557   // D2. [Initialize j.]  Set j to m. This is the loop counter over the places.
01558   int j = m;
01559   do {
01560     DEBUG(dbgs() << "KnuthDiv: quotient digit #" << j << '\n');
01561     // D3. [Calculate q'.].
01562     //     Set qp = (u[j+n]*b + u[j+n-1]) / v[n-1]. (qp=qprime=q')
01563     //     Set rp = (u[j+n]*b + u[j+n-1]) % v[n-1]. (rp=rprime=r')
01564     // Now test if qp == b or qp*v[n-2] > b*rp + u[j+n-2]; if so, decrease
01565     // qp by 1, inrease rp by v[n-1], and repeat this test if rp < b. The test
01566     // on v[n-2] determines at high speed most of the cases in which the trial
01567     // value qp is one too large, and it eliminates all cases where qp is two
01568     // too large.
01569     uint64_t dividend = ((uint64_t(u[j+n]) << 32) + u[j+n-1]);
01570     DEBUG(dbgs() << "KnuthDiv: dividend == " << dividend << '\n');
01571     uint64_t qp = dividend / v[n-1];
01572     uint64_t rp = dividend % v[n-1];
01573     if (qp == b || qp*v[n-2] > b*rp + u[j+n-2]) {
01574       qp--;
01575       rp += v[n-1];
01576       if (rp < b && (qp == b || qp*v[n-2] > b*rp + u[j+n-2]))
01577         qp--;
01578     }
01579     DEBUG(dbgs() << "KnuthDiv: qp == " << qp << ", rp == " << rp << '\n');
01580 
01581     // D4. [Multiply and subtract.] Replace (u[j+n]u[j+n-1]...u[j]) with
01582     // (u[j+n]u[j+n-1]..u[j]) - qp * (v[n-1]...v[1]v[0]). This computation
01583     // consists of a simple multiplication by a one-place number, combined with
01584     // a subtraction.
01585     // The digits (u[j+n]...u[j]) should be kept positive; if the result of
01586     // this step is actually negative, (u[j+n]...u[j]) should be left as the
01587     // true value plus b**(n+1), namely as the b's complement of
01588     // the true value, and a "borrow" to the left should be remembered.
01589     int64_t borrow = 0;
01590     for (unsigned i = 0; i < n; ++i) {
01591       uint64_t p = uint64_t(qp) * uint64_t(v[i]);
01592       int64_t subres = int64_t(u[j+i]) - borrow - (unsigned)p;
01593       u[j+i] = (unsigned)subres;
01594       borrow = (p >> 32) - (subres >> 32);
01595       DEBUG(dbgs() << "KnuthDiv: u[j+i] = " << u[j+i]
01596                    << ", borrow = " << borrow << '\n');
01597     }
01598     bool isNeg = u[j+n] < borrow;
01599     u[j+n] -= (unsigned)borrow;
01600 
01601     DEBUG(dbgs() << "KnuthDiv: after subtraction:");
01602     DEBUG(for (int i = m+n; i >=0; i--) dbgs() << " " << u[i]);
01603     DEBUG(dbgs() << '\n');
01604 
01605     // D5. [Test remainder.] Set q[j] = qp. If the result of step D4 was
01606     // negative, go to step D6; otherwise go on to step D7.
01607     q[j] = (unsigned)qp;
01608     if (isNeg) {
01609       // D6. [Add back]. The probability that this step is necessary is very
01610       // small, on the order of only 2/b. Make sure that test data accounts for
01611       // this possibility. Decrease q[j] by 1
01612       q[j]--;
01613       // and add (0v[n-1]...v[1]v[0]) to (u[j+n]u[j+n-1]...u[j+1]u[j]).
01614       // A carry will occur to the left of u[j+n], and it should be ignored
01615       // since it cancels with the borrow that occurred in D4.
01616       bool carry = false;
01617       for (unsigned i = 0; i < n; i++) {
01618         unsigned limit = std::min(u[j+i],v[i]);
01619         u[j+i] += v[i] + carry;
01620         carry = u[j+i] < limit || (carry && u[j+i] == limit);
01621       }
01622       u[j+n] += carry;
01623     }
01624     DEBUG(dbgs() << "KnuthDiv: after correction:");
01625     DEBUG(for (int i = m+n; i >=0; i--) dbgs() << " " << u[i]);
01626     DEBUG(dbgs() << "\nKnuthDiv: digit result = " << q[j] << '\n');
01627 
01628   // D7. [Loop on j.]  Decrease j by one. Now if j >= 0, go back to D3.
01629   } while (--j >= 0);
01630 
01631   DEBUG(dbgs() << "KnuthDiv: quotient:");
01632   DEBUG(for (int i = m; i >=0; i--) dbgs() <<" " << q[i]);
01633   DEBUG(dbgs() << '\n');
01634 
01635   // D8. [Unnormalize]. Now q[...] is the desired quotient, and the desired
01636   // remainder may be obtained by dividing u[...] by d. If r is non-null we
01637   // compute the remainder (urem uses this).
01638   if (r) {
01639     // The value d is expressed by the "shift" value above since we avoided
01640     // multiplication by d by using a shift left. So, all we have to do is
01641     // shift right here. In order to mak
01642     if (shift) {
01643       unsigned carry = 0;
01644       DEBUG(dbgs() << "KnuthDiv: remainder:");
01645       for (int i = n-1; i >= 0; i--) {
01646         r[i] = (u[i] >> shift) | carry;
01647         carry = u[i] << (32 - shift);
01648         DEBUG(dbgs() << " " << r[i]);
01649       }
01650     } else {
01651       for (int i = n-1; i >= 0; i--) {
01652         r[i] = u[i];
01653         DEBUG(dbgs() << " " << r[i]);
01654       }
01655     }
01656     DEBUG(dbgs() << '\n');
01657   }
01658   DEBUG(dbgs() << '\n');
01659 }