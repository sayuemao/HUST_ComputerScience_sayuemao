package hust.cs.javacourse.search.run;

import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.AbstractIndexSearcher;
import hust.cs.javacourse.search.query.Sort;
import hust.cs.javacourse.search.query.impl.IndexSearcher;
import hust.cs.javacourse.search.query.impl.SimpleSorter;
import hust.cs.javacourse.search.util.Config;

import javax.swing.plaf.nimbus.AbstractRegionPainter;

/**
 * 测试搜索
 */
// public class TestSearchIndex {
//     /**
//      * 搜索程序入口
//      * 
//      * @param args ：命令行参数
//      */
//     public static void main(String[] args) {

//     }
// }

public class TestSearchIndex {
    public static void main(String[] args) {
        Sort simpleSorter = new SimpleSorter();
        String indexFile = Config.INDEX_DIR + "index.dat";
        AbstractIndexSearcher searcher = new IndexSearcher();
        searcher.open(indexFile);
        AbstractHit[] hits = searcher.search(new Term("google"), simpleSorter);
        for (AbstractHit hit : hits) {
            System.out.println(hit);
        }
        System.out.println("-------------------------------------\n\n");
        hits = searcher.search(new Term("government"),new Term("wales"), simpleSorter, AbstractIndexSearcher.LogicalCombination.AND);
        for (AbstractHit hit : hits) {
            System.out.println(hit);
        }
        System.out.println("-------------------------------------\n\n");
        hits = searcher.search(new Term("recognition"),new Term("google"), simpleSorter, AbstractIndexSearcher.LogicalCombination.OR);
        for (AbstractHit hit : hits) {
            System.out.println(hit);
        }
        System.out.println("-------------------------------------\n\n");
        hits = ((IndexSearcher)searcher).search(new Term("facial") ,new Term("recognition"), simpleSorter);
        for (AbstractHit hit : hits) {
            System.out.println(hit);
        }
    }
}
