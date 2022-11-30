package grgr.adventofcode2017;

import java.util.Arrays;
import java.util.stream.Collectors;

import org.junit.jupiter.api.Test;

public class Day03Test {

    int input = 312051;

    @Test
    public void test1() throws Exception {
        int N = -1, M = -1;
        for (int n = 1; n < 281; n++) {
            int ss = sizeOfSquareSide(n);
            int[] mm = minAndMax(n);
            int[][] ranges = ranges(mm);
            System.out.print(n + ": " + sizeOfFullSquare(n) + ": " + sizeOfSquareCircumference(n) + ": "
                    + sizeOfSquareSide(n) + ": "
                    + Arrays.stream(mm).mapToObj(Integer::toString).collect(Collectors.joining("-")) + ": ");
            for (int side = 0; side < 4; side++) {
                int[] s = ranges[side];
                System.out.print(s[0] + "-" + s[1]);
                if (side < 3) {
                    System.out.print("|");
                }
                if (s[0] <= input && s[1] >= input) {
                    N = n - 1;
                    M = Math.abs(s[0] + ((ss - 1) / 2 - input));
                }
            }
            System.out.println();
        }
        System.out.println("N: " + N + ", M: " + M + " = " + (M+N));
    }

    @Test
    public void test2() throws Exception {
        int SIZE = 27;
//        int SIZE = 3;
        int[][] tab = new int[SIZE + 2][];
        for (int n = 0; n < SIZE + 2; n++) {
            tab[n] = new int[SIZE + 2];
            Arrays.fill(tab[n], 0);
        }

        int sx = (SIZE+1)/2, sy = (SIZE+1)/2;
        tab[sx][sy] = 1;

//        for (int r = 0; r < SIZE+2; r++) {
//            for (int c = 0; c < SIZE+2; c++) {
//                System.out.printf("\t%07d", tab[r][c]);
//            }
//            System.out.println();
//        }

        int v = 0;
        int square = 1;
        while (true) {
            square++;
            int size = sizeOfSquareSide(square) - 1;
            System.out.println("S: " + size);
            System.out.println("x: " + sx + ", y: " + sy);
            if (++sx > SIZE) {
                break;
            }
            for (int step = 0; step < size; step++) {
                System.out.printf("1: x=%d, y=%d\n", sx, sy);
                v = fill(tab, sx, sy);
                sy--;
                if (v > input) {
                    break;
                }
            }
            if (v > input) {
                break;
            }
            sy++;sx--;
            for (int step = 0; step < size; step++) {
                System.out.printf("2: x=%d, y=%d\n", sx, sy);
                v = fill(tab, sx, sy);
                sx--;
                if (v > input) {
                    break;
                }
            }
            if (v > input) {
                break;
            }
            sx++;sy++;
            for (int step = 0; step < size; step++) {
                System.out.printf("3: x=%d, y=%d\n", sx, sy);
                v = fill(tab, sx, sy);
                sy++;
                if (v > input) {
                    break;
                }
            }
            if (v > input) {
                break;
            }
            sy--;sx++;
            for (int step = 0; step < size; step++) {
                System.out.printf("4: x=%d, y=%d\n", sx, sy);
                v = fill(tab, sx, sy);
                sx++;
                if (v > input) {
                    break;
                }
            }
            if (v > input) {
                break;
            }
            sx--;
        }
        for (int r = 0; r < SIZE+2; r++) {
            for (int c = 0; c < SIZE+2; c++) {
                System.out.printf("\t%07d", tab[r][c]);
            }
            System.out.println();
        }

        System.out.println(v);
    }

    private int fill(int[][] tab, int sx, int sy) {
        tab[sx][sy]
                = tab[sx + 1][sy - 1]
                + tab[sx + 1][sy]
                + tab[sx + 1][sy + 1]
                + tab[sx][sy - 1]
                + tab[sx][sy + 1]
                + tab[sx - 1][sy - 1]
                + tab[sx - 1][sy]
                + tab[sx - 1][sy + 1];
        return tab[sx][sy];
    }

    int sizeOfFullSquare(int n) {
        return (n + n - 1) * (n + n - 1);
    }

    int sizeOfSquareCircumference(int n) {
        return n == 1 ? 1 : sizeOfFullSquare(n) - sizeOfFullSquare(n - 1);
    }

    int sizeOfSquareSide(int n) {
        return n == 1 ? 1 : sizeOfSquareCircumference(n) / 4 + 1;
    }

    int[] minAndMax(int n) {
        return n == 1 ? new int[] { 1, 1 } : new int[] { sizeOfFullSquare(n - 1) + 1, sizeOfFullSquare(n) };
    }

    int[][] ranges(int[] mm) {
        int min = mm[0], max = mm[1];
        int[][] res = new int[4][];
        for (int i = 0; i < 4; i++) {
            res[i] = new int[] { min + (i * (max-min)/4), min + ((i + 1) * (max-min)/4) };
        }
        return res;
    }

}
