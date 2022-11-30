package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

import org.junit.jupiter.api.Test;

public class Day23Test {

    String input1 = "set a 1\n" +
            "add a 2\n" +
            "mul a a\n" +
            "mod a 5\n" +
            "snd a\n" +
            "set a 0\n" +
            "rcv a\n" +
            "jgz a -1\n" +
            "set a 1\n" +
            "jgz a -2\n";
    String input2 = "set b 79\n" +
            "set c b\n" +
            "jnz a 2\n" +
            "jnz 1 5\n" +
            "mul b 100\n" +
            "sub b -100000\n" +
            "set c b\n" +
            "sub c -17000\n" +
            "set f 1\n" +
            "set d 2\n" +
            "set e 2\n" +
            "set g d\n" +
            "mul g e\n" +
            "sub g b\n" +
            "jnz g 2\n" +
            "set f 0\n" +
            "sub e -1\n" +
            "set g e\n" +
            "sub g b\n" +
            "jnz g -8\n" +
            "sub d -1\n" +
            "set g d\n" +
            "sub g b\n" +
            "jnz g -13\n" +
            "jnz f 2\n" +
            "sub h -1\n" +
            "set g b\n" +
            "sub g c\n" +

            "jnz g 2\n" +
            "jnz 1 3\n" +
            "sub b -17\n" +
            "jnz 1 -23\n";

    @Test
    public void test1() throws Exception {
        Map<String, Long> registers = new HashMap<>();
        String line = null;
        BufferedReader reader = new BufferedReader(new StringReader(input2));

        List<String[]> program = new ArrayList<>();
        while ((line = reader.readLine()) != null) {
            String[] exec = line.split(" +");
            program.add(exec);
            registers.putIfAbsent(exec[1], 0L);
            if (exec.length > 2 && Character.isAlphabetic(exec[2].charAt(0))) {
                registers.putIfAbsent(exec[2], 0L);
            }
        }

        long ip = 0;
        long muls = 0;
        out:
        while (true) {
            String[] exec = program.get((int) ip);
            String ins = exec[0];
            String reg = exec[1];
            Long arg1 = null;
            long arg2 = 0;
            if (!Character.isAlphabetic(exec[1].charAt(0))) {
                arg1 = Long.parseLong(exec[1]);
            }
            if (exec.length > 2) {
                if (Character.isAlphabetic(exec[2].charAt(0))) {
                    // register
                    arg2 = registers.get(exec[2]);
                } else {
                    arg2 = Long.parseLong(exec[2]);
                }
            }
            System.out.printf("> %s %s %d\n", ins, reg, arg2);
            long ipadd = 1;
            switch (ins) {
                case "set":
                    registers.put(reg, arg2);
                    break;
                case "sub":
                    registers.put(reg, registers.get(reg) - arg2);
                    break;
                case "mul":
                    muls++;
                    registers.put(reg, registers.get(reg) * arg2);
                    break;
                case "jnz":
                    if ((arg1 != null && arg1 != 0) || registers.get(reg) != 0) {
                        ipadd = arg2;
                    }
                    break;
            }
            ip += ipadd;
            if (ip < 0 || ip > program.size() - 1) {
                break;
            }
        }

        System.out.println(registers);
        System.out.println(muls);
    }

    @Test
    public void test2() throws Exception {
        Map<String, Long> registers = new HashMap<>();
        String line = null;
        BufferedReader reader = new BufferedReader(new StringReader(input2));

        List<String[]> program = new ArrayList<>();
        while ((line = reader.readLine()) != null) {
            String[] exec = line.split(" +");
            program.add(exec);
            registers.putIfAbsent(exec[1], 0L);
            if (exec.length > 2 && Character.isAlphabetic(exec[2].charAt(0))) {
                registers.putIfAbsent(exec[2], 0L);
            }
        }
        registers.put("a", 1L);

        long ip = 0;
        long muls = 0;
        out:
        while (true) {
            String[] exec = program.get((int) ip);
            String ins = exec[0];
            String reg = exec[1];
            Long arg1 = null;
            long arg2 = 0;
            if (!Character.isAlphabetic(exec[1].charAt(0))) {
                arg1 = Long.parseLong(exec[1]);
            }
            if (exec.length > 2) {
                if (Character.isAlphabetic(exec[2].charAt(0))) {
                    // register
                    arg2 = registers.get(exec[2]);
                } else {
                    arg2 = Long.parseLong(exec[2]);
                }
            }
            System.out.printf("%2d> %s %s %s=%d\n", ip, ins, reg, exec[2], arg2);
            long ipadd = 1;
            switch (ins) {
                case "set":
                    registers.put(reg, arg2);
                    break;
                case "sub":
                    registers.put(reg, registers.get(reg) - arg2);
                    break;
                case "mul":
                    muls++;
                    registers.put(reg, registers.get(reg) * arg2);
                    break;
                case "jnz":
                    if ((arg1 != null && arg1 != 0) || registers.get(reg) != 0) {
                        ipadd = arg2;
                    }
                    break;
            }
            ip += ipadd;
            if (ip < 0 || ip > program.size() - 1) {
                break;
            }
        }

        System.out.println(registers);
        System.out.println(muls);
    }

    @Test
    public void test2a() throws Exception {
        System.out.println(method());
    }

    int method() {
        int h = 0;
        int b = 107900;
        int c = 124900;
//        if (a != 0) {
//            b *= 100;
//            b += 100000;
//            c = b;
//            c += 17000;
//        }
        System.out.println("b = " + b);
        System.out.println("c = " + c);
        while (true) {
            boolean found = new BigInteger(Integer.toString(b)).isProbablePrime(10000000);
//            int d = 2;
//            while (true) {
//                int e = 2;
//                while (true) {
//                    if (d * e == b) {
//                        found = true;
//                    }
//                    e++;
//                    if (e == b) {
//                        break;
//                    }
//                }
//                d++;
//                if (d == b) {
//                    break;
//                }
//            }

            if (!found) {
                h++;
            }
            if (b == c) {
                return h;
            }
            b += 17;
        }
    }

}
