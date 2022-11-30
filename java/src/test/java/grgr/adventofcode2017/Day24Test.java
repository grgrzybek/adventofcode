package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringReader;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.junit.jupiter.api.Test;

import static jdk.nashorn.internal.objects.Global.print;

public class Day24Test {

    String input1 = "0/2\n" +
            "2/2\n" +
            "2/3\n" +
            "3/4\n" +
            "3/5\n" +
            "0/1\n" +
            "10/1\n" +
            "9/10\n";
    String input2 = "25/13\n" +
            "4/43\n" +
            "42/42\n" +
            "39/40\n" +
            "17/18\n" +
            "30/7\n" +
            "12/12\n" +
            "32/28\n" +
            "9/28\n" +
            "1/1\n" +
            "16/7\n" +
            "47/43\n" +
            "34/16\n" +
            "39/36\n" +
            "6/4\n" +
            "3/2\n" +
            "10/49\n" +
            "46/50\n" +
            "18/25\n" +
            "2/23\n" +
            "3/21\n" +
            "5/24\n" +
            "46/26\n" +
            "50/19\n" +
            "26/41\n" +
            "1/50\n" +
            "47/41\n" +
            "39/50\n" +
            "12/14\n" +
            "11/19\n" +
            "28/2\n" +
            "38/47\n" +
            "5/5\n" +
            "38/34\n" +
            "39/39\n" +
            "17/34\n" +
            "42/16\n" +
            "32/23\n" +
            "13/21\n" +
            "28/6\n" +
            "6/20\n" +
            "1/30\n" +
            "44/21\n" +
            "11/28\n" +
            "14/17\n" +
            "33/33\n" +
            "17/43\n" +
            "31/13\n" +
            "11/21\n" +
            "31/39\n" +
            "0/9\n" +
            "13/50\n" +
            "10/14\n" +
            "16/10\n" +
            "3/24\n" +
            "7/0\n" +
            "50/50\n";

    @Test
    public void test1() throws Exception {
        Map<String, int[]> components = new HashMap<>();
        String line = null;
        BufferedReader reader = new BufferedReader(new StringReader(input2));
        List<String> roots = new LinkedList<>();
        while ((line = reader.readLine()) != null) {
            String[] ports = line.split("/");
            if (Integer.parseInt(ports[0]) > Integer.parseInt(ports[1])) {
                line = ports[1] + "/" + ports[0];
                ports = line.split("/");
            }
            int[] c = new int[] { Integer.parseInt(ports[0]), Integer.parseInt(ports[1]) };
            if (Integer.parseInt(ports[0]) == 0) {
                roots.add(line);
            }
            components.put(line, c);
        }

        List<Node> rootNodes = new LinkedList<>();
        for (String root : roots) {
            Map<String, int[]> _components = new HashMap<>(components);
            Node child = new Node(_components.get(root)[0], _components.get(root)[1]);
            _components.remove(root);
            rootNodes.add(child);
            constructTree(child, _components);
        }

//        print(rootNodes, 0);

        int max = 0;
        for (Node root : rootNodes) {
            int s = str(root);
            if (max < s) {
                max = s;
            }
        }

        System.out.println(max);
    }

    @Test
    public void test2() throws Exception {
        Map<String, int[]> components = new HashMap<>();
        String line = null;
        BufferedReader reader = new BufferedReader(new StringReader(input2));
        List<String> roots = new LinkedList<>();
        while ((line = reader.readLine()) != null) {
            String[] ports = line.split("/");
            if (Integer.parseInt(ports[0]) > Integer.parseInt(ports[1])) {
                line = ports[1] + "/" + ports[0];
                ports = line.split("/");
            }
            int[] c = new int[] { Integer.parseInt(ports[0]), Integer.parseInt(ports[1]) };
            if (Integer.parseInt(ports[0]) == 0) {
                roots.add(line);
            }
            components.put(line, c);
        }

        List<Node> rootNodes = new LinkedList<>();
        for (String root : roots) {
            Map<String, int[]> _components = new HashMap<>(components);
            Node child = new Node(_components.get(root)[0], _components.get(root)[1]);
            _components.remove(root);
            rootNodes.add(child);
            constructTree(child, _components);
        }

        try (FileWriter wr = new FileWriter(new File("/data/tmp/tree.txt"))) {
            print(wr, rootNodes, 0);
        }

        int c = 0;
        int lc = 1;
        int ln = 0;
        try (BufferedReader br = new BufferedReader(new FileReader("/data/tmp/tree.txt"))) {
            String l = null;
            while ((l = br.readLine()) != null) {
                if (c < l.length()) {
                    c = l.length();
                    ln = lc;
                }
                lc++;
            }
        }
        System.out.println("L: " + c + ", N: " + ln);

        int maxL = 0;
        int maxS = 0;
        for (Node root : rootNodes) {
            int[] s = str2(root);
            if (maxL < s[0] && maxS < s[1]) {
                maxL = s[0];
                maxS = s[1];
            }
        }

        System.out.println(maxL + " : " + maxS);
    }

    private void print(FileWriter wr, List<Node> nodes, int indent) throws IOException {
        for (Node n : nodes) {
            for (int i = 0; i < indent; i++) {
                wr.write(" ");
            }
            wr.write(String.format("%04d/%04d\n", n.in, n.out));
            print(wr, n.connected, indent + 9);
        }
    }

    private void constructTree(Node root, Map<String, int[]> components) {
        Map<String, int[]> leaves = new HashMap<>();
        for (String c : components.keySet()) {
            if (c.startsWith(root.out + "/") || c.endsWith("/" + root.out)) {
                leaves.put(c, components.get(c));
            }
        }
        for (String c : leaves.keySet()) {
            Map<String, int[]> _components = new HashMap<>(components);
            _components.keySet().remove(c);
            Node child = new Node(root.out == leaves.get(c)[0] ? leaves.get(c)[0] : leaves.get(c)[1],
                    root.out == leaves.get(c)[0] ? leaves.get(c)[1] : leaves.get(c)[0]);
            root.connected.add(child);
            constructTree(child, _components);
        }
    }

    private int str(Node root) {
        int r = root.in + root.out;
        int max = 0;
        for (Node n : root.connected) {
            int x = str(n);
            if (max < x) {
                max = x;
            }
        }
        return r + max;
    }

    private int[] str2(Node root) {
        int l = 1;
        int s = root.in + root.out;
        int maxL = 0;
        int maxS = 0;
        for (Node n : root.connected) {
            int[] x = str2(n);
            if (maxL <= x[0] && maxS < x[1]) {
                maxL = x[0];
                maxS = x[1];
            }
        }
        return new int [] { l + maxL, s + maxS };
    }

    public static class Node {
        boolean taken = false;
        int in = -1;
        int out = -1;
        List<Node> connected = new LinkedList<>();

        public Node(int in, int out) {
            this.in = in;
            this.out = out;
        }
    }

}
