package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.Sort;

import java.util.Collections;
//import java.util.Comparator;
import java.util.List;
import java.util.Map;

/**
 * SimpleSorter is a concrete implementation of Sort interface.
 * Provides simple sorting and scoring strategy
 * 凭Term出现频率freq从大到小排序
 */
public class SimpleSorter implements Sort {

    @Override
    public void sort(List<AbstractHit> hits) {
        Collections.sort(hits);
        Collections.reverse(hits);
    }

    @Override
    public double score(AbstractHit hit) {
        double totalScore = 0;
        for (Map.Entry<AbstractTerm, AbstractPosting> entry : hit.getTermPostingMapping().entrySet()) {
            totalScore += entry.getValue().getFreq();
        }
        hit.setScore(totalScore);
        return totalScore;
    }
}
