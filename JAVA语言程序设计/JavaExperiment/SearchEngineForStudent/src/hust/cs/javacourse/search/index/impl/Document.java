package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractDocument;
import hust.cs.javacourse.search.index.AbstractTermTuple;

import java.util.ArrayList;
import java.util.List;

public class Document extends AbstractDocument {

    public Document() {
    }

    /**
     * Constructor
     * 
     * @param docId:   document id
     * @param docPath: document absolute path
     */
    public Document(int docId, String docPath) {
        this.docId = docId;
        this.docPath = docPath;
        this.tuples = new ArrayList<>();
    }

    /**
     * Constructor
     * 
     * @param docId:   document id
     * @param docPath: document absolute path
     * @param tuples:  list of term tuples
     */
    public Document(int docId, String docPath, List<AbstractTermTuple> tuples) {
        this.docId = docId;
        this.docPath = docPath;
        this.tuples = tuples != null ? new ArrayList<>(tuples) : new ArrayList<>();
    }

    @Override
    public int getDocId() {
        return docId;
    }

    @Override
    public void setDocId(int docId) {
        this.docId = docId;
    }

    @Override
    public String getDocPath() {
        return docPath;
    }

    @Override
    public void setDocPath(String docPath) {
        this.docPath = docPath;
    }

    @Override
    public List<AbstractTermTuple> getTuples() {
        return tuples;
    }

    @Override
    public void addTuple(AbstractTermTuple tuple) {
        if (!contains(tuple)) {
            tuples.add(tuple);
        }
    }

    @Override
    public boolean contains(AbstractTermTuple tuple) {
        return tuples.contains(tuple);
    }

    @Override
    public AbstractTermTuple getTuple(int index) {
        return tuples.get(index);
    }

    @Override
    public int getTupleSize() {
        return tuples.size();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Document{");
        sb.append("docId=").append(docId);
        sb.append(", docPath='").append(docPath).append('\'');
        sb.append(", tuples=").append(tuples);
        sb.append('}');
        return sb.toString();
    }
}
