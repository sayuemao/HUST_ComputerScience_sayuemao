package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.index.impl.TermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleScanner;
import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.StringSplitter;

import java.io.BufferedReader;
import java.io.IOException;
//import java.io.InputStreamReader;
//import java.io.FileReader;
import java.util.List;

/**
 * TermTupleScanner is a concrete implementation of AbstractTermTupleScanner.
 * Used to read text files and generate term tuple streams
 */
public class TermTupleScanner extends AbstractTermTupleScanner {

    private StringSplitter splitter = new StringSplitter();
    private List<String> words = null;
    private int position = 0;

    public TermTupleScanner() {
        super();
        splitter.setSplitRegex(Config.STRING_SPLITTER_REGEX);
    }

    public TermTupleScanner(BufferedReader input) {
        super(input);
        splitter.setSplitRegex(Config.STRING_SPLITTER_REGEX);
        splitFileContent();
    }

    private void splitFileContent() {
        try {
            StringBuilder content = new StringBuilder();
            String line;
            while ((line = input.readLine()) != null) {
                content.append(line).append(" ");
            }
            words = splitter.splitByRegex(content.toString());
            position = 0;
        } catch (IOException e) {
            e.printStackTrace();
            words = new java.util.ArrayList<>();
        }
    }

    @Override
    public AbstractTermTuple next() {
        if (words == null || position >= words.size()) {
            return null;
        }

        String word = words.get(position);
        if (Config.IGNORE_CASE) {
            word = word.toLowerCase();
        }

        TermTuple tuple = new TermTuple();
        tuple.term = new Term(word);
        tuple.curPos = position;
        position++;

        return tuple;
    }
}
