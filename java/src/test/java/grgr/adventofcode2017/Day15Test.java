package grgr.adventofcode2017;

import org.junit.jupiter.api.Test;

public class Day15Test {

    long[] values1_ = new long[] { 65L, 8921L };
    long[] values1 = new long[] { 873L, 583L };

    @Test
    public void test1() throws Exception {
        int matches = 0;
        for (int i = 0; i < 40000000; i++) {
            if (i % 1000000 == 0) {
                System.out.println(values1[0] + " | " + values1[1]);
            }
            matches += compare();
            values1[0] *= 16807L;
            values1[0] %= 0x7fffffff;
            values1[1] *= 48271L;
            values1[1] %= 0x7fffffff;
        }
        System.out.println(matches);
    }

    @Test
    public void test2() throws Exception {
        int matches = 0;
        for (int i = 0; i < 5000000; i++) {
            while ((values1[0] & 3) != 0) {
                values1[0] *= 16807L;
                values1[0] %= 0x7fffffff;
            }
            while ((values1[1] & 7) != 0) {
                values1[1] *= 48271L;
                values1[1] %= 0x7fffffff;
            }
            if (i % 1000000 == 0) {
                System.out.println(values1[0] + " | " + values1[1]);
            }
            matches += compare();
            values1[0] *= 16807L;
            values1[0] %= 0x7fffffff;
            values1[1] *= 48271L;
            values1[1] %= 0x7fffffff;
        }
        System.out.println(matches);
    }

    private int compare() {
        return (values1[0] & 0xffff) == (values1[1] & 0xffff) ? 1 : 0;
    }

}
