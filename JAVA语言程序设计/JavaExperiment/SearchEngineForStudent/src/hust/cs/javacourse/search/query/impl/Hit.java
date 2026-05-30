package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import java.util.Map;
//import java.util.TreeMap;

/**
 * Hit is a concrete implementation of AbstractHit.
 * Represents a search result hit
 */
public class Hit extends AbstractHit {

    public Hit() {
    }

    public Hit(int docId, String docPath) {
        super(docId, docPath);
    }

    public Hit(int docId, String docPath, Map<AbstractTerm, AbstractPosting> termPostingMapping) {
        super(docId, docPath, termPostingMapping);
    }

    @Override
    public int getDocId() {
        return docId;
    }

    @Override
    public String getDocPath() {
        return docPath;
    }

    @Override
    public String getContent() {
        return content;
    }

    @Override
    public void setContent(String content) {
        this.content = content;
    }

    @Override
    public double getScore() {
        return score;
    }

    @Override
    public void setScore(double score) {
        this.score = score;
    }

    @Override
    public Map<AbstractTerm, AbstractPosting> getTermPostingMapping() {
        return termPostingMapping;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("------------------------------------------------------\n");
        sb.append("docId: ").append(docId).append("\n");
        sb.append("docPath: ").append(docPath).append("\n");
        sb.append("content: ").append(content).append("\n");
        sb.append("score: ").append(score).append("\n");
        sb.append("TermPostingMapping: \n");
        if (termPostingMapping != null) {
            for (Map.Entry<AbstractTerm, AbstractPosting> entry : termPostingMapping.entrySet()) {
                sb.append(entry.getKey().toString()).append(" ----> ").append(entry.getValue().toString()).append("\n");
            }
        }
        return sb.toString();
    }

    @Override
    public int compareTo(AbstractHit o) {
        return (int)(this.getScore() - o.getScore());
    }
}
