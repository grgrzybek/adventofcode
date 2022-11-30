package grgr.adventofcode2017;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.junit.jupiter.api.Test;

public class Day06Test {

    String input = "10\t3\t15\t10\t5\t15\t5\t15\t9\t2\t5\t8\t5\t2\t3\t6";
//    String input = "0\t2\t7\t0";

    @Test
    public void test1() throws Exception {
        Integer[] tab = Arrays.stream(input.split("\t")).map(Integer::parseInt).toArray(Integer[]::new);
        Set<String> collect = new HashSet<>();
        int count = 0;
        while (true) {
            String v = Arrays.stream(tab).map(i -> Integer.toString(i)).collect(Collectors.joining("|"));
//            System.out.println(count + " : " + v);
            if (!collect.add(v)) {
                break;
            }
            count++;
            int idx = -1, max = -1;
            for (int i = 0; i < tab.length; i ++) {
                if (tab[i] > max) {
                    max = tab[i];
                    idx = i;
                }
            }
            tab[idx] = 0;
            int idxc = idx + 1;
            for (int i = 0; i < max; i++) {
                tab[idxc++ % tab.length]++;
            }
        }
        System.out.println("count: " + count);
    }

    @Test
    public void test2() throws Exception {
        Integer[] tab = Arrays.stream(input.split("\t")).map(Integer::parseInt).toArray(Integer[]::new);
        Map<String, Integer> collect = new HashMap<>();
        int c1 = 0;
        int count = 0;
        while (true) {
            String v = Arrays.stream(tab).map(i -> Integer.toString(i)).collect(Collectors.joining("|"));
//            System.out.println(count + " : " + v);
            Integer previous = collect.put(v, count);
            if (previous != null) {
                c1 = previous;
                break;
            }
            count++;
            int idx = -1, max = -1;
            for (int i = 0; i < tab.length; i ++) {
                if (tab[i] > max) {
                    max = tab[i];
                    idx = i;
                }
            }
            tab[idx] = 0;
            int idxc = idx + 1;
            for (int i = 0; i < max; i++) {
                tab[idxc++ % tab.length]++;
            }
        }
        System.out.println("count: " + (count - c1));
    }

}
