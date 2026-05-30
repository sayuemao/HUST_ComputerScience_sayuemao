package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractDocument;
import hust.cs.javacourse.search.index.AbstractDocumentBuilder;
import hust.cs.javacourse.search.index.AbstractIndex;
import hust.cs.javacourse.search.index.AbstractIndexBuilder;
//import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.FileUtil;

import java.io.File;
import java.util.List;

/**
 * IndexBuilder is a concrete implementation of AbstractIndexBuilder.
 * Used to build inverted index
 */
public class IndexBuilder extends AbstractIndexBuilder {

    public IndexBuilder(AbstractDocumentBuilder docBuilder) {
        super(docBuilder);
    }

    @Override
    public AbstractIndex buildIndex(String rootDirectory) {
        Index index = new Index();
        List<String> filePaths = FileUtil.list(rootDirectory, ".txt");

        for (String filePath : filePaths) {
            AbstractDocument document = docBuilder.build(docId, filePath, new File(filePath));
            index.addDocument(document);
            docId++;
        }

        index.optimize();

        return index;
    }
}
