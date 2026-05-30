package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;

import java.util.regex.Pattern;

//基于正则表达式的过滤器
public class PatternTermTupleFilter extends AbstractTermTupleFilter {

    private Pattern pattern = null;

    public PatternTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
        this.pattern = Pattern.compile(Config.TERM_FILTER_PATTERN);
    }

    @Override
    public AbstractTermTuple next() {
        while (true) {
            AbstractTermTuple tuple = input.next();
            if (tuple == null) {
                return null;
            }
            if (pattern.matcher(tuple.term.getContent()).matches()) {
                return tuple;
            }
        }
    }
}
