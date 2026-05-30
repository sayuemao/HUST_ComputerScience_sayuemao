package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.*;
import hust.cs.javacourse.search.util.Config;

import java.io.*;
import java.util.Map;
import java.util.Set;

/**
 * AbstractIndex的具体实现类
 */
public class Index extends AbstractIndex {
    /**
     * 返回索引的字符串表示
     *
     * @return 索引的字符串表示
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Index{\n");
        sb.append("  docId--docPath mapping:\n");
        for(Map.Entry<Integer, String> entry : docIdToDocPathMapping.entrySet()){
            sb.append("    ").append(entry.getKey()).append(" -> ").append(entry.getValue()).append("\n");
        }
        sb.append("  PostingList:\n");
        for (Map.Entry<AbstractTerm, AbstractPostingList> entry : termToPostingListMapping.entrySet()) {
            sb.append("    ").append(entry.getKey()).append(" -> ").append(entry.getValue()).append('\n');
        }
        sb.append("\n");
        sb.append('}');
        return sb.toString();
    }
    
    /**
     * 添加文档到索引，更新索引内部的HashMap
     *
     * @param document ：文档的AbstractDocument子类型表示
     */
    @Override
    public void addDocument(AbstractDocument document) {
        docIdToDocPathMapping.put(document.getDocId(), document.getDocPath());

        for (int i = 0; i < document.getTupleSize(); i++) {
            AbstractTermTuple tuple = document.getTuple(i);
            AbstractTerm term = tuple.term;

            if (Config.IGNORE_CASE) {
                term = new Term(term.getContent().toLowerCase());
            }

            AbstractPostingList postingList = termToPostingListMapping.get(term);
            if (postingList == null) {
                postingList = new PostingList();
                termToPostingListMapping.put(term, postingList);
            }

            int index = postingList.indexOf(document.getDocId());
            if (index == -1) {
                Posting posting = new Posting(document.getDocId(), tuple.freq, new java.util.ArrayList<>());
                posting.getPositions().add(tuple.curPos);
                postingList.add(posting);
            } else {
                AbstractPosting posting = postingList.get(index);
                posting.setFreq(posting.getFreq() + tuple.freq);
                posting.getPositions().add(tuple.curPos);
            }
        }
    }
    
    /**
     * <pre>
     * 从索引文件里加载已经构建好的索引.内部调用FileSerializable接口方法readObject即可
     * @param file ：索引文件
     * </pre>
     */
    @Override
    public void load(File file) {
        try (ObjectInputStream in = new ObjectInputStream(new FileInputStream(file))) {
            readObject(in);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * <pre>
     * 将在内存里构建好的索引写入到文件. 内部调用FileSerializable接口方法writeObject即可
     * @param file ：写入的目标索引文件
     * </pre>
     */
    @Override
    public void save(File file) {
        try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(file))) {
            writeObject(out);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 返回指定单词的PostingList
     *
     * @param term : 指定的单词
     * @return ：指定单词的PostingList;如果索引字典没有该单词，则返回null
     */
    @Override
    public AbstractPostingList search(AbstractTerm term) {
        return termToPostingListMapping.get(term);
    }

    /**
     * 返回索引的字典.字典为索引里所有单词的并集
     *
     * @return ：索引中Term列表
     */
    @Override
    public Set<AbstractTerm> getDictionary() {
        return termToPostingListMapping.keySet();
    }
    
    /**
     * <pre>
     * 对索引进行优化，包括：
     *      对索引里每个单词的PostingList按docId从小到大排序
     *      同时对每个Posting里的positions从小到大排序
     * 在内存中把索引构建完后执行该方法
     * </pre>
     */
    @Override
    public void optimize() {
        for (AbstractPostingList postingList : termToPostingListMapping.values()) {
            postingList.sort();
        }
    }
    
    /**
     * 根据docId获得对应文档的完全路径名
     *
     * @param docId ：文档id
     * @return : 对应文档的完全路径名
     */
    @Override
    public String getDocName(int docId) {
        return docIdToDocPathMapping.get(docId);
    }

    /**
     * 写到二进制文件
     *
     * @param out :输出流对象
     */
    @Override
    public void writeObject(ObjectOutputStream out) {
        try {
            out.writeObject(this.termToPostingListMapping);
            out.writeObject(this.docIdToDocPathMapping);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 从二进制文件读
     *
     * @param in ：输入流对象
     */
    @Override
    public void readObject(ObjectInputStream in) {
        try{
            this.termToPostingListMapping = (Map<AbstractTerm, AbstractPostingList>) in.readObject();
            this.docIdToDocPathMapping = (Map<Integer, String>) in.readObject();
        }catch (IOException | ClassNotFoundException e){
            e.printStackTrace();
        }
    }
}
