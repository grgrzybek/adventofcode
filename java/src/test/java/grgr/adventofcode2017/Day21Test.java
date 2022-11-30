package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.junit.jupiter.api.Test;

public class Day21Test {

    String input1 = "../.# => ##./#../...\n" +
            ".#./..#/### => #..#/..../..../#..#\n";
    String input2 = "../.. => #.#/.#./...\n" +
            "#./.. => .../..#/..#\n" +
            "##/.. => .#./##./###\n" +
            ".#/#. => ..#/#../##.\n" +
            "##/#. => ##./..#/#.#\n" +
            "##/## => ###/###/.##\n" +
            ".../.../... => ...#/#.##/.##./#..#\n" +
            "#../.../... => .###/#.##/##.#/##.#\n" +
            ".#./.../... => #.../.###/..#./#.##\n" +
            "##./.../... => .#.#/.###/##../#.##\n" +
            "#.#/.../... => .##./..../##.#/#...\n" +
            "###/.../... => ..##/.#.#/###./#..#\n" +
            ".#./#../... => .#.#/##.#/.#.#/.##.\n" +
            "##./#../... => ###./.###/#.../...#\n" +
            "..#/#../... => #.##/##../.#../.##.\n" +
            "#.#/#../... => ..##/..../..##/...#\n" +
            ".##/#../... => ####/#..#/.#../....\n" +
            "###/#../... => ##../..#./##../....\n" +
            ".../.#./... => .###/...#/#.../#...\n" +
            "#../.#./... => .#../.#.#/..##/#.#.\n" +
            ".#./.#./... => ##.#/####/.#.#/##..\n" +
            "##./.#./... => ..../.###/#.#./..##\n" +
            "#.#/.#./... => ..#./..#./..../....\n" +
            "###/.#./... => ###./..../..#./....\n" +
            ".#./##./... => ..../.##./##.#/....\n" +
            "##./##./... => ..#./.#../..##/#...\n" +
            "..#/##./... => #.##/.##./..#./.##.\n" +
            "#.#/##./... => .###/#.../##.#/..#.\n" +
            ".##/##./... => ###./##../..#./..##\n" +
            "###/##./... => ..#./.##./.###/#..#\n" +
            ".../#.#/... => ###./#.../####/#.#.\n" +
            "#../#.#/... => .##./.#../#.##/.#..\n" +
            ".#./#.#/... => .#../##../..##/.#.#\n" +
            "##./#.#/... => ###./#.#./##.#/##..\n" +
            "#.#/#.#/... => #.../.##./#.##/#.#.\n" +
            "###/#.#/... => ###./..##/#.##/###.\n" +
            ".../###/... => ##../...#/#.#./#.#.\n" +
            "#../###/... => .#../...#/##.#/####\n" +
            ".#./###/... => #.#./..##/#.#./.##.\n" +
            "##./###/... => ..##/..##/.###/#...\n" +
            "#.#/###/... => ####/##../..../#..#\n" +
            "###/###/... => ...#/#.##/#.##/#.#.\n" +
            "..#/.../#.. => .#.#/..##/#.##/#..#\n" +
            "#.#/.../#.. => ...#/..#./##../#..#\n" +
            ".##/.../#.. => ####/##../..../##..\n" +
            "###/.../#.. => ..#./..#./##.#/#..#\n" +
            ".##/#../#.. => .#../####/.###/#..#\n" +
            "###/#../#.. => ####/.#.#/...#/..##\n" +
            "..#/.#./#.. => #.#./.##./####/....\n" +
            "#.#/.#./#.. => ##../###./.#../##..\n" +
            ".##/.#./#.. => ###./.#../...#/....\n" +
            "###/.#./#.. => .#../.###/##../##.#\n" +
            ".##/##./#.. => .#../#..#/.###/#...\n" +
            "###/##./#.. => ..../.##./##../...#\n" +
            "#../..#/#.. => ##.#/...#/.###/##.#\n" +
            ".#./..#/#.. => ##../##../..../#.#.\n" +
            "##./..#/#.. => ..##/.#../#.#./.#.#\n" +
            "#.#/..#/#.. => ..../..##/...#/...#\n" +
            ".##/..#/#.. => #.../..##/...#/####\n" +
            "###/..#/#.. => #.../..#./##.#/.#.#\n" +
            "#../#.#/#.. => ..##/#.../#..#/..#.\n" +
            ".#./#.#/#.. => #..#/#.../.##./#.##\n" +
            "##./#.#/#.. => ##.#/.##./##.#/...#\n" +
            "..#/#.#/#.. => ####/.#.#/.##./#.#.\n" +
            "#.#/#.#/#.. => #..#/.##./.##./.###\n" +
            ".##/#.#/#.. => ...#/...#/..../.##.\n" +
            "###/#.#/#.. => .#../###./..../.###\n" +
            "#../.##/#.. => ##.#/##../#.#./...#\n" +
            ".#./.##/#.. => ###./.#.#/#.##/####\n" +
            "##./.##/#.. => #.##/..#./.#../#..#\n" +
            "#.#/.##/#.. => #.#./..##/..##/.#.#\n" +
            ".##/.##/#.. => .#../.###/.###/#.##\n" +
            "###/.##/#.. => #.../##../#.#./.#..\n" +
            "#../###/#.. => #.#./###./.##./..#.\n" +
            ".#./###/#.. => #.../#.../.##./.#..\n" +
            "##./###/#.. => ..#./.###/..##/#...\n" +
            "..#/###/#.. => #.##/.#../###./.###\n" +
            "#.#/###/#.. => .#.#/#..#/###./##..\n" +
            ".##/###/#.. => #.#./#.##/..##/.#..\n" +
            "###/###/#.. => ##../#.../..#./#..#\n" +
            ".#./#.#/.#. => #..#/####/#.#./#..#\n" +
            "##./#.#/.#. => ..##/.#../##.#/#..#\n" +
            "#.#/#.#/.#. => ####/#.#./#..#/#.#.\n" +
            "###/#.#/.#. => #.../##.#/..../#...\n" +
            ".#./###/.#. => ..##/.##./####/.###\n" +
            "##./###/.#. => .##./..#./#.##/#..#\n" +
            "#.#/###/.#. => ##.#/##../####/...#\n" +
            "###/###/.#. => ..##/####/...#/.#..\n" +
            "#.#/..#/##. => #.##/.#.#/#.#./#.##\n" +
            "###/..#/##. => ...#/##.#/#..#/..#.\n" +
            ".##/#.#/##. => .#.#/..#./..../###.\n" +
            "###/#.#/##. => ###./####/##.#/#.##\n" +
            "#.#/.##/##. => ##.#/#.##/.##./##..\n" +
            "###/.##/##. => .#.#/#.#./###./####\n" +
            ".##/###/##. => .#../####/.#../....\n" +
            "###/###/##. => .#../..../##.#/.##.\n" +
            "#.#/.../#.# => #.../#.../..##/..##\n" +
            "###/.../#.# => ...#/..#./##.#/####\n" +
            "###/#../#.# => .###/..##/.#../....\n" +
            "#.#/.#./#.# => ###./####/.#../#..#\n" +
            "###/.#./#.# => #.../#.##/..../###.\n" +
            "###/##./#.# => .###/####/#..#/.###\n" +
            "#.#/#.#/#.# => .#.#/...#/.#.#/#.##\n" +
            "###/#.#/#.# => ..../..#./..#./####\n" +
            "#.#/###/#.# => ..##/...#/.#.#/.##.\n" +
            "###/###/#.# => .###/.##./..##/####\n" +
            "###/#.#/### => #.#./.#../.#.#/#.#.\n" +
            "###/###/### => #..#/##../#.#./####\n";

    @Test
    public void test1() throws Exception {
        List<String> image = new ArrayList<>();
        image.add(".#./..#/###");

        System.out.println(split(Arrays.asList("abcd/efgh/ijkl/mnop|ABCD/EFGH/IJKL/MNOP", "abcd/efgh/ijkl/mnop|ABCD/EFGH/IJKL/MNOP")));
        Map<String, String> instructions = new HashMap<>();
        BufferedReader reader = new BufferedReader(new StringReader(input2));
        String line = null;
        while ((line = reader.readLine()) != null) {
            String[] ins = line.split(" *=> *");
            Map<String, String> m = new HashMap<>();
            for (String patt : patterns(ins[0])) {
                m.put(patt, ins[1]);
            }
            instructions.putAll(m);
        }

//        System.out.println(image);
        for (int it = 0; it < 18; it++) {
            image = split(image);
            List<String> nimage = new LinkedList<>();
            for (String il : image) {
                String[] blocks = il.split("\\|");
                nimage.add(Arrays.stream(blocks).map(instructions::get).collect(Collectors.joining("|")));
            }
            image = nimage;
        }
        String s = image.stream().collect(Collectors.joining("."));
        System.out.println(s.replaceAll("[^#]*", "").length());
    }

    // gets all the patters for single pattern
    // 01 30 43 14
    // 34 41 10 03
    //
    // 012 840 a98 26a
    // 456 951 654 159
    // 89a a62 210 048
    private List<String> patterns(String in1) {
        List<String> list = new LinkedList<>();
        list.add(in1);
        String[] t = in1.split("/");
        if (t.length == 2) {
            String in2 = t[1] + "/" + t[0];
            list.add(in2);
            list.add(new String(new char[] { in1.charAt(3), in1.charAt(0), '/', in1.charAt(4), in1.charAt(1) }));
            list.add(new String(new char[] { in1.charAt(4), in1.charAt(3), '/', in1.charAt(1), in1.charAt(0) }));
            list.add(new String(new char[] { in1.charAt(1), in1.charAt(4), '/', in1.charAt(0), in1.charAt(3) }));
            list.add(new String(new char[] { in2.charAt(3), in2.charAt(0), '/', in2.charAt(4), in2.charAt(1) }));
            list.add(new String(new char[] { in2.charAt(4), in2.charAt(3), '/', in2.charAt(1), in2.charAt(0) }));
            list.add(new String(new char[] { in2.charAt(1), in2.charAt(4), '/', in2.charAt(0), in2.charAt(3) }));
        } else if (t.length == 3) {
            String in2 = t[2] + "/" + t[1] + "/" + t[0];
            list.add(in2);
            list.add(new String(new char[] { in1.charAt(8), in1.charAt(4), in1.charAt(0), '/', in1.charAt(9), in1.charAt(5), in1.charAt(1), '/', in1.charAt(10), in1.charAt(6), in1.charAt(2) }));
            list.add(new String(new char[] { in1.charAt(10), in1.charAt(9), in1.charAt(8), '/', in1.charAt(6), in1.charAt(5), in1.charAt(4), '/', in1.charAt(2), in1.charAt(1), in1.charAt(0) }));
            list.add(new String(new char[] { in1.charAt(2), in1.charAt(6), in1.charAt(10), '/', in1.charAt(1), in1.charAt(5), in1.charAt(9), '/', in1.charAt(0), in1.charAt(4), in1.charAt(8) }));
            list.add(new String(new char[] { in2.charAt(8), in2.charAt(4), in2.charAt(0), '/', in2.charAt(9), in2.charAt(5), in2.charAt(1), '/', in2.charAt(10), in2.charAt(6), in2.charAt(2) }));
            list.add(new String(new char[] { in2.charAt(10), in2.charAt(9), in2.charAt(8), '/', in2.charAt(6), in2.charAt(5), in2.charAt(4), '/', in2.charAt(2), in2.charAt(1), in2.charAt(0) }));
            list.add(new String(new char[] { in2.charAt(2), in2.charAt(6), in2.charAt(10), '/', in2.charAt(1), in2.charAt(5), in2.charAt(9), '/', in2.charAt(0), in2.charAt(4), in2.charAt(8) }));
        }
        return list;
    }

    // splits to 2x2 or 3x3 images
    private List<String> split(List<String> image) {
//        System.out.println("1) " + image);
        List<String> nimage = new ArrayList<>();
        for (String line : image) {
            String[] blocks = line.split("\\|");
            String[] side = blocks[0].split("/");
            for (int i = 0; i < side.length; i++) {
                StringWriter sw = new StringWriter();
                for (String b : blocks) {
                    sw.write(b.split("/")[i]);
                }
                nimage.add(sw.toString());
            }
        }
//        System.out.println("2) " + nimage);

        List<String> rimage = new ArrayList<>();
        if (nimage.get(0).length() % 2 == 0) {
            /*
                abcdABCD
                efghEFGH
                ijklIJKL
                mnopMNOP
                abcdABCD
                efghEFGH
                ijklIJKL
                mnopMNOP
                ↓
                ab/ef|cd/gh|AB/EF|CD/GH
                ab/ef|cd/gh|AB/EF|CD/GH
             */
            for (int i = 0; i < nimage.get(0).length() / 2; i++) {
                StringWriter sw = new StringWriter();
                for (int j = 0; j < nimage.get(0).length() / 2; j++) {
                    sw.append("|");
                    sw.append(nimage.get(i*2).substring(j*2, j*2+2));
                    sw.append("/");
                    sw.append(nimage.get(i*2+1).substring(j*2, j*2+2));
                }
                rimage.add(sw.toString().substring(1));
            }
        } else if (nimage.get(0).length() % 3 == 0) {
            for (int i = 0; i < nimage.get(0).length() / 3; i++) {
                StringWriter sw = new StringWriter();
                for (int j = 0; j < nimage.get(0).length() / 3; j++) {
                    sw.append("|");
                    sw.append(nimage.get(i*3).substring(j*3, j*3+3));
                    sw.append("/");
                    sw.append(nimage.get(i*3+1).substring(j*3, j*3+3));
                    sw.append("/");
                    sw.append(nimage.get(i*3+2).substring(j*3, j*3+3));
                }
                rimage.add(sw.toString().substring(1));
            }
        }

//        System.out.println("3) " + rimage);
        return rimage;
    }

}
