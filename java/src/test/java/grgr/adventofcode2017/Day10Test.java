package grgr.adventofcode2017;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Deque;
import java.util.LinkedList;
import java.util.List;

import org.junit.jupiter.api.Test;

public class Day10Test {

    String input1 = "3,4,1,5";
    String input = "227,169,3,166,246,201,0,47,1,255,2,254,96,3,97,144";

    @Test
    public void test1() throws Exception {
        int[] tab1 = new int[] { 0, 1, 2, 3, 4 };
        int[] tab = new int[256];
        for (int i = 0; i < 256; i++) {
            tab[i] = i;
        }
        tab = tab1;
        input = input1;

        int skip = 0;
        int pos = 0;
        int[] lengths = Arrays.stream(input.split(", *")).mapToInt(Integer::parseInt).toArray();
        for (int l : lengths) {
            int[] sub = new int[l];
            for (int i = pos; i < pos + l; i++) {
                sub[i-pos] = tab[i % tab.length];
            }
            List<Integer> list = new ArrayList<>();
            for (int aSub : sub) {
                list.add(aSub);
            }
            Collections.reverse(list);
            for (int i = pos; i < pos + l; i++) {
                tab[i%tab.length] = list.get(i-pos);
            }
            pos += l + skip;
            skip++;
        }
        for (int aTab : tab) {
            System.out.print(aTab + " ");
        }
        System.out.println();
        System.out.println(tab[0] * tab[1]);
    }

    @Test
    public void test2() throws Exception {
        int[] tab = new int[256];
        for (int i = 0; i < 256; i++) {
            tab[i] = i;
        }
//        input = input1;
//        input = "1,2,4";

        List<Integer> lengths = new ArrayList<>();
        char[] in = input.toCharArray();
        for (char anIn : in) {
            lengths.add(0 + anIn);
        }
        lengths.addAll(Arrays.asList(17, 31, 73, 47, 23));
        System.out.println(lengths);

        int skip = 0;
        int pos = 0;
        for (int r = 0; r < 64; r++) {
            for (int l : lengths) {
                int[] sub = new int[l];
                for (int i = pos; i < pos + l; i++) {
                    sub[i-pos] = tab[i % tab.length];
                }
                List<Integer> list = new ArrayList<>();
                for (int aSub : sub) {
                    list.add(aSub);
                }
                Collections.reverse(list);
                for (int i = pos; i < pos + l; i++) {
                    tab[i%tab.length] = list.get(i-pos);
                }
                pos += l + skip;
                skip++;
            }
        }
        int[] dense = new int[16];
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                dense[i] ^= tab[i * 16 + j];
            }
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 16; i++) {
            sb.append(Integer.toHexString(dense[i]));
        }
        System.out.println(sb.toString());
//        System.out.println(tab[0] * tab[1]);
    }

}
