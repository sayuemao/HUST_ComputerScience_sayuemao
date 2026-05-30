package hust.cs.javacourse.search.index.impl;

//import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.AbstractTermTuple;

/**
 * TermTuple is a concrete implementation of AbstractTermTuple.
 * Represents a tuple of (term, frequency, current position)
 */
public class TermTuple extends AbstractTermTuple {

    public TermTuple() {
        term = new Term();
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null || getClass() != obj.getClass())
            return false;
        TermTuple termTuple = (TermTuple) obj;
        return freq == termTuple.freq &&
                curPos == termTuple.curPos &&
                term.equals(termTuple.term);
    }

    @Override
    public String toString() {
        return "TermTuple{" +
                "term=" + term +
                ", freq=" + freq +
                ", curPos=" + curPos +
                '}';
    }
}
