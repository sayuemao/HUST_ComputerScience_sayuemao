package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.StopWords;

//import java.util.Arrays;

//Í£ÓÃ´Ê¹ýÂËÆ÷
public class StopWordTermTupleFilter extends AbstractTermTupleFilter {

    public StopWordTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
    }

    @Override
    public AbstractTermTuple next() {
        while (true) {
            AbstractTermTuple tuple = input.next();
            if (tuple == null) {
                return null;
            }
            String word = Config.IGNORE_CASE ? tuple.term.getContent().toLowerCase() : tuple.term.getContent();
            boolean isStopWord = false;
            for (String stopWord : StopWords.STOP_WORDS) {
                if (Config.IGNORE_CASE) {
                    if (word.equalsIgnoreCase(stopWord)) {
                        isStopWord = true;
                        break;
                    }
                } else {
                    if (word.equals(stopWord)) {
                        isStopWord = true;
                        break;
                    }
                }
            }
            if (!isStopWord) {
                return tuple;
            }
        }
    }
}
