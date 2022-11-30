package grgr.adventofcode2017;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;

import org.junit.jupiter.api.Test;

public class Day17Test {

    int v1 = 3;
    int v2 = 335;

    @Test
    public void test1() throws Exception {
        List<Integer> cb = new ArrayList<>();
        cb.add(0);

        int cp = 0;
        for (int i = 1; i <= 2017; i++) {
            for (int s = 0; s < v2; s++) {
                cp++;
            }
            cp %= cb.size();
            cb.add(++cp, i);
        }
        System.out.println(cb.get(cp+1%cb.size()));
    }

    @Test
    public void test2() throws Exception {
        int[] cb = new int[50000001];
        int[] _cb = new int[50000001];
//        int[] cb = new int[2018];
//        int[] _cb = new int[2018];
        int cs = 1;
        cb[cs-1] = 0;

        int cp = 0;
        for (int i = 1; i <= 50000000; i++) {
            if (i % 100000 == 0) {
                System.out.println(i);
                System.out.println("CP : " + cb[0] + "|" + cb[1]);
            }
            for (int s = 0; s < v2; s++) {
                cp++;
            }
            cp %= cs;
            System.arraycopy(cb, 0, _cb, 0, cp+1);
            _cb[++cp] = i;
            System.arraycopy(cb, cp, _cb, cp+1, cs - cp);
//            System.out.println(cb);
            cs++;
            int[] _ = cb;
            cb = _cb;
            _cb = _;
        }
        for (int i = 0 ; i <= 50000000; i++) {
            if (cb[i] == 0) {
                System.out.println(cb[i+1%cb.length]);
            }
        }
        //        System.out.println(Arrays.stream(cb, 0, cs).mapToObj(ix -> Integer.toString(ix)).collect(Collectors.joining(",")));
    }

    @Test
    public void test2a() throws Exception {
        LinkedList<Integer> cb = new LinkedList<>();
        cb.add(0);

        int cp = 0;
        for (int i = 1; i <= 2017; i++) {
            if (i < 10) {
                System.out.println(i + ": " + cb);
//                System.out.println("CP : " + cb.get(0) + "|" + cb.get(1));
            }
            cp+=v2;
            cp %= cb.size();
            cb.add(++cp, i);
        }
        for (int i = 0 ; i <= 2017; i++) {
            if (cb.get(i) == 0) {
                System.out.println(cb.get(i+1%cb.size()));
            }
        }
    }

    @Test
    public void test2b() throws Exception {
        int currentPosition = 0;
        int afterZero = -1;
        for (int i = 1; i <= 50000001; i++) {
            currentPosition += v2+1;
            currentPosition %= i;
            if (currentPosition == 0) {
                System.out.println("> " + i);
                afterZero = i;
            }
        }
        System.out.println(afterZero);
    }

}
