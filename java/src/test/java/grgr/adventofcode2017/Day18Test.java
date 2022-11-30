package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
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

public class Day18Test {

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
    String input1b = "snd 1\n" +
            "snd 2\n" +
            "snd p\n" +
            "rcv a\n" +
            "rcv b\n" +
            "rcv c\n" +
            "rcv d\n";
    String input2 = "set i 31\n" +
            "set a 1\n" +
            "mul p 17\n" +
            "jgz p p\n" +
            "mul a 2\n" +
            "add i -1\n" +
            "jgz i -2\n" +
            "add a -1\n" +
            "set i 127\n" +
            "set p 464\n" +
            "mul p 8505\n" +
            "mod p a\n" +
            "mul p 129749\n" +
            "add p 12345\n" +
            "mod p a\n" +
            "set b p\n" +
            "mod b 10000\n" +
            "snd b\n" +
            "add i -1\n" +
            "jgz i -9\n" +
            "jgz a 3\n" +
            "rcv b\n" +
            "jgz b -1\n" +
            "set f 0\n" +
            "set i 126\n" +
            "rcv a\n" +
            "rcv b\n" +
            "set p a\n" +
            "mul p -1\n" +
            "add p b\n" +
            "jgz p 4\n" +
            "snd a\n" +
            "set a b\n" +
            "jgz 1 3\n" +
            "snd b\n" +
            "set f 1\n" +
            "add i -1\n" +
            "jgz i -11\n" +
            "snd a\n" +
            "jgz f -16\n" +
            "jgz a -19\n";

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
        long sound = 0;
        long recovered = 0;
        out:
        while (true) {
            String[] exec = program.get((int) ip);
            String ins = exec[0];
            String reg = exec[1];
            long arg2 = 0;
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
                case "snd":
                    sound = registers.get(reg);
                    break;
                case "set":
                    registers.put(reg, arg2);
                    break;
                case "add":
                    registers.put(reg, registers.get(reg) + arg2);
                    break;
                case "mul":
                    registers.put(reg, registers.get(reg) * arg2);
                    break;
                case "mod":
                    registers.put(reg, registers.get(reg) % arg2);
                    break;
                case "rcv":
                    if (registers.get(reg) > 0) {
                        recovered = sound;
                        break out;
                    }
                    break;
                case "jgz":
                    if (registers.get(reg) > 0) {
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
        System.out.println(ip + ": " + recovered);
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
            if (Character.isAlphabetic(exec[1].charAt(0))) {
                registers.putIfAbsent(exec[1], 0L);
            }
            if (exec.length > 2 && Character.isAlphabetic(exec[2].charAt(0))) {
                registers.putIfAbsent(exec[2], 0L);
            }
        }

        LinkedBlockingQueue<Long> q0 = new LinkedBlockingQueue<>();
        LinkedBlockingQueue<Long> q1 = new LinkedBlockingQueue<>();
        Program p0 = new Program(registers, program, 0, q0, q1);
        Program p1 = new Program(registers, program, 1, q1, q0);

        CompletionService<Integer> cs = new ExecutorCompletionService<>(Executors.newFixedThreadPool(2));
        cs.submit(p0, 11);
        cs.submit(p1, 12);

        Future f1 = cs.take();
        Future f2 = cs.take();
        f1.get();
        f2.get();

        System.out.println(p1.sends);
    }

    private class Program extends Thread {

        private final long id;
        private final LinkedBlockingQueue<Long> sender;
        private final LinkedBlockingQueue<Long> receiver;
        private Map<String, Long> registers;
        private List<String[]> program;

        int sends = 0;

        public Program(Map<String, Long> registers, List<String[]> program, long id, LinkedBlockingQueue<Long> sender, LinkedBlockingQueue<Long> receiver) {
            this.registers = new HashMap<>(registers);
            this.registers.put("p", id);
            this.program = new ArrayList<>(program);
            this.id = id;
            this.sender = sender;
            this.receiver = receiver;
        }

        @Override
        public void run() {
            try {
                long ip = 0;
                System.out.println(id + ": " + registers);
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
//                    printx(this.id, ip, ins, reg, arg2);
                    long ipadd = 1;
                    switch (ins) {
                        case "snd":
                            sender.put(registers.get(reg));
                            sends++;
                            break;
                        case "set":
                            registers.put(reg, arg2);
                            break;
                        case "add":
                            registers.put(reg, registers.get(reg) + arg2);
                            break;
                        case "mul":
                            registers.put(reg, registers.get(reg) * arg2);
                            break;
                        case "mod":
                            registers.put(reg, registers.get(reg) % arg2);
                            break;
                        case "rcv":
                            Long v = null;
                            if ((v = receiver.poll(10, TimeUnit.SECONDS)) == null) {
                                break out;
                            }
                            registers.put(reg, v);
                            break;
                        case "jgz":
                            if ((arg1 != null && arg1 > 0) || registers.get(reg) > 0) {
                                ipadd = arg2;
                            }
                            break;
                    }
                    ip += ipadd;
                    if (ip < 0 || ip > program.size() - 1) {
                        break;
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private synchronized void printx(long id, long ip, String ins, String reg, long arg2) {
        System.out.printf("%d (%d)> %s %s %d\n", id, ip, ins, reg, arg2);
    }

}
