package grgr.adventofcode2017;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.MemoryImageSource;
import java.awt.image.RenderedImage;
import java.io.BufferedWriter;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import javax.imageio.ImageIO;

import org.junit.jupiter.api.Test;
import sun.awt.image.ToolkitImage;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class Day14Test {

    String input1 = "flqrgnkx";
    String input2 = "vbqugkhl";

    @Test
    public void test1() throws Exception {
        int count = 0;
        for (int i = 0; i < 128; i++) {
            String kh = knotHash(input2 + "-" + i);
            for (char c : kh.toCharArray()) {
                count += Integer.toBinaryString(Integer.parseInt("" + c, 16)).replaceAll("0", "").length();
            }
        }
        System.out.println(count);
    }

    @Test
    public void test2() throws Exception {
        int count = 0;
        int[][] grid = new int[128][];
        for (int i = 0; i < 128; i++) {
            grid[i] = new int[128];
            for (int j = 0; j < 128; j++) {
                grid[i][j] = 0;
            }
        }

        for (int i = 0; i < 128; i++) {
            String kh = knotHash(input2 + "-" + i);
            StringBuilder sb = new StringBuilder();
            int idx = 0;
            for (char c : kh.toCharArray()) {
                String s = Integer.toBinaryString(Integer.parseInt("" + c, 16));
                if (s.length() < 4) {
                    s = "0000".substring(0, 4 - s.length()) + s;
                }
                for (char c2 : s.toCharArray()) {
                    grid[i][idx++] = c2 == '0' ? 0 : 1;
                }
            }
            assertTrue(idx == 128);
        }

        int img = 1;
        toImage(grid, img++);

        int regions = 0;
        int MARKER = 2;
        while (true) {
            int x = -1;
            int y = -1;
            boolean change = false;
            for (int i = 0; i < 128; i++) {
                for (int j = 0; j < 128; j++) {
                    if (grid[i][j] == 1) {
                        if (x == -1 && y == -1) {
                            // first '1'
                            x = i; y = j;
                            grid[i][j] = MARKER;
                            regions++;
                            change = true;
                        }
                    }
                }
            }
            if (!change) {
                break;
            } else {
                while (true) {
                    boolean change2 = false;
                    for (int i = 0; i < 128; i++) {
                        for (int j = 0; j < 128; j++) {
                            if (grid[i][j] == 1) {
                                if (i < 127 && grid[i + 1][j] == MARKER) {
                                    grid[i][j] = MARKER;
                                    change2 = true;
                                }
                                if (i > 0 && grid[i - 1][j] == MARKER) {
                                    grid[i][j] = MARKER;
                                    change2 = true;
                                }
                                if (j > 0 && grid[i][j - 1] == MARKER) {
                                    grid[i][j] = MARKER;
                                    change2 = true;
                                }
                                if (j < 127 && grid[i][j + 1] == MARKER) {
                                    grid[i][j] = MARKER;
                                    change2 = true;
                                }
                            }
                        }
                    }
                    if (!change2) {
                        break;
                    }
                }
            }
            if (img < 10) {
                toImage(grid, img++);
            }
            MARKER++;
            if (!change) {
                break;
            }
        }
        System.out.println(regions + " | " + MARKER);
    }

    private void toImage(int[][] grid, int name) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(new File(String.format("/data/tmp/img-%03d.txt", name))));
        for (int i = 0; i < 128; i++) {
            for (int j = 0; j < 128; j++) {
                bw.write(grid[i][j] == 1 ? "#" : "x");
            }
            bw.write("\n");
        }
        bw.close();
    }

    public String knotHash(String input) throws Exception {
        int[] tab = new int[256];
        for (int i = 0; i < 256; i++) {
            tab[i] = i;
        }

        List<Integer> lengths = new ArrayList<>();
        char[] in = input.toCharArray();
        for (char anIn : in) {
            lengths.add(0 + anIn);
        }
        lengths.addAll(Arrays.asList(17, 31, 73, 47, 23));

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
            sb.append(String.format("%02x", dense[i]));
        }
        return sb.toString();
    }

}
