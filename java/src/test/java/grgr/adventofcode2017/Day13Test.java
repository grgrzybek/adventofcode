package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.junit.jupiter.api.Test;

public class Day13Test {

    String input1 = "0: 3\n" +
            "1: 2\n" +
            "4: 4\n" +
            "6: 4\n";
    String input2 = "0: 4\n" +
            "1: 2\n" +
            "2: 3\n" +
            "4: 4\n" +
            "6: 8\n" +
            "8: 5\n" +
            "10: 6\n" +
            "12: 6\n" +
            "14: 10\n" +
            "16: 8\n" +
            "18: 6\n" +
            "20: 9\n" +
            "22: 8\n" +
            "24: 6\n" +
            "26: 8\n" +
            "28: 8\n" +
            "30: 12\n" +
            "32: 12\n" +
            "34: 12\n" +
            "36: 12\n" +
            "38: 10\n" +
            "40: 12\n" +
            "42: 12\n" +
            "44: 14\n" +
            "46: 8\n" +
            "48: 14\n" +
            "50: 12\n" +
            "52: 14\n" +
            "54: 14\n" +
            "58: 14\n" +
            "60: 12\n" +
            "62: 14\n" +
            "64: 14\n" +
            "66: 12\n" +
            "68: 12\n" +
            "72: 14\n" +
            "74: 18\n" +
            "76: 17\n" +
            "86: 14\n" +
            "88: 20\n" +
            "92: 14\n" +
            "94: 14\n" +
            "96: 18\n" +
            "98: 18\n";

    @Test
    public void test1() throws Exception {
        Map<Integer, int[]> firewall = new HashMap<>();

        BufferedReader reader = new BufferedReader(new StringReader(input2));
        String line = null;
        int max = -1;
        int severity = 0;
        while ((line = reader.readLine()) != null) {
            String[] exp = line.split(": ");
            firewall.put(Integer.parseInt(exp[0]), new int[] { 0, Integer.parseInt(exp[1])-1, 1 });
            max = Integer.parseInt(exp[0]);
        }
        for (int ps = 0; ps <= max; ps++) {
            System.out.printf("===\n%03d\n", ps);
            print(max, firewall);
            if (firewall.containsKey(ps)) {
                if (firewall.get(ps)[0] == 0) {
                    severity += ps * (firewall.get(ps)[1] + 1);
                }
            }
            for (int ps2 = 0; ps2 <= max; ps2++) {
                if (!firewall.containsKey(ps2)) {
                    continue;
                }
                if (firewall.get(ps2)[0] == 0 && firewall.get(ps2)[2] == -1) {
                    firewall.get(ps2)[2] *= -1;
                } else if (firewall.get(ps2)[0] == firewall.get(ps2)[1] && firewall.get(ps2)[2] == 1) {
                    firewall.get(ps2)[2] *= -1;
                }
                firewall.get(ps2)[0] += firewall.get(ps2)[2];
            }
            print(max, firewall);
        }

        System.out.println(severity);
    }

    @Test
    public void test2() throws Exception {
        Map<Integer, int[]> _firewall = new HashMap<>();
        BufferedReader reader = new BufferedReader(new StringReader(input2));
        String line = null;
        int max = -1;
        while ((line = reader.readLine()) != null) {
            String[] exp = line.split(": ");
            _firewall.put(Integer.parseInt(exp[0]), new int[] { 0, Integer.parseInt(exp[1])-1, 1 });
            max = Integer.parseInt(exp[0]);
        }

        int severity = 0;
        int attepmts = 0;
        int delay = 0;
        while (true) {
            Map<Integer, int[]> firewall = new HashMap<>();
            _firewall.forEach((n, t) -> {
                firewall.put(n, Arrays.copyOf(t, t.length));
            });
            attepmts++;
            severity = 0;
            boolean caught = false;
//            System.out.println("======== " + delay + " ========");
            for (int ps2 = 0; ps2 <= max; ps2++) {
                int[] ints = firewall.get(ps2);
                if (ints == null) {
                    continue;
                }
                skip(ints, delay);
            }
            for (int ps = 0; ps <= max; ps++) {
                int[] ints = firewall.get(ps);
                if (ints != null && ints[0] == 0) {
                    caught = true;
                    severity += ps * (ints[1] + 1);
                    break;
                }
                for (int ps2 = 0; ps2 <= max; ps2++) {
                    if (!firewall.containsKey(ps2)) {
                        continue;
                    }
    //                print(max, firewall, ps);
                    int[] rule = firewall.get(ps2);
                    if (rule[0] == 0 && rule[2] == -1) {
                        rule[2] *= -1;
                    } else if (rule[0] == rule[1] && rule[2] == 1) {
                        rule[2] *= -1;
                    }
                    rule[0] += rule[2];
                }
            }
            if (delay % 100000 == 0) {
                System.out.println("delay: " + delay + ", severity: " + severity);
            }
            if (!caught) {
                break;
            }
            delay++;
        }

        System.out.println(attepmts);
    }

    private void skip(int[] rule, int delay) {
        for (int ps = 0; ps < delay % ((rule[1] + 1)*2 - 2); ps++) {
            if (rule[0] == 0 && rule[2] == -1) {
                rule[2] *= -1;
            } else if (rule[0] == rule[1] && rule[2] == 1) {
                rule[2] *= -1;
            }
            rule[0] += rule[2];
        }
    }

    void print(int max, Map<Integer, int[]> fw) {
        for (int i = 0; i <= max; i++) {
            if (!fw.containsKey(i)) {
                System.out.print("... ");
            } else {
                System.out.printf("%03d ", fw.get(i)[0]);
            }
        }
        System.out.println();
    }

    void print(int max, Map<Integer, int[]> fw, int ps) {
        for (int i = 0; i <= max; i++) {
            if (i == ps) {
                System.out.print("(");
            } else {
                System.out.print(" ");
            }
            if (!fw.containsKey(i)) {
                System.out.print("...");
            } else {
                System.out.printf("%03d", fw.get(i)[0]);
            }
            if (i == ps) {
                System.out.print(")");
            } else {
                System.out.print(" ");
            }
        }
        System.out.println();
    }

}
