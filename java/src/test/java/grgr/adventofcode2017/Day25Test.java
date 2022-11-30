package grgr.adventofcode2017;

import java.util.ArrayList;
import java.util.stream.Collectors;

import org.junit.jupiter.api.Test;

public class Day25Test {

    @Test
    public void test1() throws Exception {
        ArrayList<Integer> tape = new ArrayList<>();

        int idx = 0;
        tape.add(0);
        State state = State.A;

        for (int i = 0; i < 12481997; i++) {
            int v = tape.get(idx);
            if (v == 0) {
                switch (state) {
                    case A:
                        tape.set(idx, 1);
                        idx = move(idx, tape, 1);
                        state = State.B;
                        break;
                    case B:
                        tape.set(idx, 1);
                        idx = move(idx, tape, -1);
                        state = State.A;
                        break;
                    case C:
                        tape.set(idx, 0);
                        idx = move(idx, tape, -1);
                        state = State.B;
                        break;
                    case D:
                        tape.set(idx, 1);
                        idx = move(idx, tape, 1);
                        state = State.A;
                        break;
                    case E:
                        tape.set(idx, 1);
                        idx = move(idx, tape, -1);
                        state = State.F;
                        break;
                    case F:
                        tape.set(idx, 1);
                        idx = move(idx, tape, 1);
                        state = State.D;
                        break;
                }
            } else if (v == 1) {
                switch (state) {
                    case A:
                        tape.set(idx, 0);
                        idx = move(idx, tape, -1);
                        state = State.C;
                        break;
                    case B:
                        tape.set(idx, 1);
                        idx = move(idx, tape, 1);
                        state = State.D;
                        break;
                    case C:
                        tape.set(idx, 0);
                        idx = move(idx, tape, -1);
                        state = State.E;
                        break;
                    case D:
                        tape.set(idx, 0);
                        idx = move(idx, tape, 1);
                        state = State.B;
                        break;
                    case E:
                        tape.set(idx, 1);
                        idx = move(idx, tape, -1);
                        state = State.C;
                        break;
                    case F:
                        tape.set(idx, 1);
                        idx = move(idx, tape, 1);
                        state = State.A;
                        break;
                }
            }
        }

        System.out.println(tape.size());
        System.out.println(tape.stream().filter(i -> i == 1).collect(Collectors.toList()).size());
    }

    private int move(int idx, ArrayList<Integer> tape, int i) {
        if (idx + i == tape.size()) {
            tape.add(0);
            return idx + i;
        }
        if (idx == 0 && i == -1) {
            tape.add(0, 0);
            return 0;
        }
        return idx + i;
    }

    private enum State {
        A, B, C, D, E, F
    }

}
