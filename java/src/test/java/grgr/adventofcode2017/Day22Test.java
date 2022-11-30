package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.junit.jupiter.api.Test;

public class Day22Test {

    String input1 = "..#\n" +
            "#..\n" +
            "...\n";
    String input2 = "......#.#....####.##.#...\n" +
            "##.##.#.####.##.#.#.##..#\n" +
            ".#.#######.#..###......#.\n" +
            "#####..###.##..####.#..##\n" +
            ".#..#.##...#.####.#....#.\n" +
            "#.#...#.#####.#.#####..##\n" +
            "..##.#..######....####.##\n" +
            "#.##.#....#.#.##........#\n" +
            ".#....#....###.#....####.\n" +
            "....#..##.#.#.##.#....#.#\n" +
            ".#.##.#.####..#..#.##..##\n" +
            "##.####.#..###...#.#...##\n" +
            "....#....#..#..####.##...\n" +
            "..#.#.#.#..#.###...#...##\n" +
            ".#..#..##..##.#.#..##.#..\n" +
            "####.#.#...##.#..##.###..\n" +
            "###.#....#...#..#..#...##\n" +
            ".##....##.......####.#.##\n" +
            "#.#.##.#.#..#.#..##..####\n" +
            "...#..##.#.####.....##.##\n" +
            ".#.##.#####.#.#....#####.\n" +
            "##......#..#.###..####.##\n" +
            "..#...#########.....#..##\n" +
            "##..###.##...###.#.#.#.#.\n" +
            "..###.###.##.#.###....#.#\n";

    @Test
    public void test1() throws Exception {
        Map<String, Boolean> grid = new HashMap<>();

        BufferedReader reader = new BufferedReader(new StringReader(input2));
        String line = null;
        int lc = 0;
        while ((line = reader.readLine()) != null) {
            int middle = (line.length() - 1) / 2;
            int cc = -middle;
            for (char c : line.toCharArray()) {
                String coord = String.format("%d:%d", cc++, middle - lc);
                grid.put(coord, c == '#');
            }
            lc++;
        }
        System.out.println(grid);

        int infections = 0;
        int dx = 0;
        int dy = 1;
        int x = 0;
        int y = 0;
        for (int i = 0; i < 10000; i++) {
            String coord = String.format("%d:%d", x, y);
            if (grid.computeIfAbsent(coord, c -> false)) {
                // infected, clean, right
                grid.put(coord, false);
                if (dy == 1) {
                    dx = 1;
                    dy = 0;
                } else if (dy == -1) {
                    dx = -1;
                    dy = 0;
                } else if (dx == 1) {
                    dx = 0;
                    dy = -1;
                } else if (dx == -1) {
                    dx = 0;
                    dy = 1;
                }
            } else {
                // clean, infect, right
                grid.put(coord, true);
                infections++;
                if (dy == 1) {
                    dx = -1;
                    dy = 0;
                } else if (dy == -1) {
                    dx = 1;
                    dy = 0;
                } else if (dx == 1) {
                    dx = 0;
                    dy = 1;
                } else if (dx == -1) {
                    dx = 0;
                    dy = -1;
                }
            }
            x += dx;
            y += dy;
        }
        System.out.println(infections);
    }

    @Test
    public void test2() throws Exception {
        Map<String, Character> grid = new HashMap<>();

        BufferedReader reader = new BufferedReader(new StringReader(input2));
        String line = null;
        int lc = 0;
        while ((line = reader.readLine()) != null) {
            int middle = (line.length() - 1) / 2;
            int cc = -middle;
            for (char c : line.toCharArray()) {
                String coord = String.format("%d:%d", cc++, middle - lc);
                grid.put(coord, c);
            }
            lc++;
        }
        System.out.println(grid);

        int infections = 0;
        int dx = 0;
        int dy = 1;
        int x = 0;
        int y = 0;
        for (int i = 0; i < 10000000; i++) {
            String coord = String.format("%d:%d", x, y);
            switch (grid.computeIfAbsent(coord, c -> '.')) {
                case '.':
                    grid.put(coord, 'W');
                    // left
                    if (dy == 1) {
                        dx = -1;
                        dy = 0;
                    } else if (dy == -1) {
                        dx = 1;
                        dy = 0;
                    } else if (dx == 1) {
                        dx = 0;
                        dy = 1;
                    } else if (dx == -1) {
                        dx = 0;
                        dy = -1;
                    }
                    break;
                case 'W':
                    grid.put(coord, '#');
                    infections++;
                    // continue
                    break;
                case '#':
                    grid.put(coord, 'F');
                    // right
                    if (dy == 1) {
                        dx = 1;
                        dy = 0;
                    } else if (dy == -1) {
                        dx = -1;
                        dy = 0;
                    } else if (dx == 1) {
                        dx = 0;
                        dy = -1;
                    } else if (dx == -1) {
                        dx = 0;
                        dy = 1;
                    }
                    break;
                case 'F':
                    grid.put(coord, '.');
                    dx = -dx;
                    dy = -dy;
                    break;
            }
            x += dx;
            y += dy;
        }
        System.out.println(infections);
    }

}
