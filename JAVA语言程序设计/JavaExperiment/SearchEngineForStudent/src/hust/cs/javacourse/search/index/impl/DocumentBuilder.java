package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractDocument;
import hust.cs.javacourse.search.index.AbstractDocumentBuilder;
import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.parse.impl.*;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

/**
 * DocumentBuilder is a concrete implementation of AbstractDocumentBuilder.
 * Used to build document objects
 */
public class DocumentBuilder extends AbstractDocumentBuilder {

    public DocumentBuilder() {
    }

    @Override
    public AbstractDocument build(int docId, String docPath, AbstractTermTupleStream termTupleStream) {
        Document document = new Document(docId, docPath);
        List<AbstractTermTuple> tuples = new ArrayList<>();

        while (true) {
            AbstractTermTuple tuple = termTupleStream.next();
            if (tuple == null) {
                break;
            }
            tuples.add(tuple);
        }

        for (AbstractTermTuple tuple : tuples) {
            document.addTuple(tuple);
        }

        return document;
    }

    /*
    // @Override
    // public AbstractDocument build(int docId, String docPath, File file) {
    //     try {
    //         BufferedReader reader = new BufferedReader(new FileReader(file));
    //         TermTupleScanner scanner = new TermTupleScanner(reader);

    //         PatternTermTupleFilter patternFilter = new PatternTermTupleFilter(scanner);
    //         LengthTermTupleFilter lengthFilter = new LengthTermTupleFilter(patternFilter);
    //         StopWordTermTupleFilter stopWordFilter = new StopWordTermTupleFilter(lengthFilter);

    //         Document document = (Document) build(docId, docPath, stopWordFilter);
    //         stopWordFilter.close();

    //         return document;
    //     } catch (IOException e) {
    //         e.printStackTrace();
    //         return null;
    //     }
    // }
     */

    @Override
    public AbstractDocument build(int docId, String docPath, File file) {
        AbstractDocument document = null;
        AbstractTermTupleStream ts = null;
        try {
            ts = new TermTupleScanner(new BufferedReader(new InputStreamReader(new FileInputStream(file))));
            ts = new StopWordTermTupleFilter(ts); // 再加上停用词过滤器
            ts = new PatternTermTupleFilter(ts); // 再加上正则表达式过滤器
            ts = new LengthTermTupleFilter(ts); // 再加上单词长度过滤器
            document = build(docId, docPath, ts);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } finally {
            if (ts != null) {
                ts.close();
            }
        }
        return document;
    }
}
