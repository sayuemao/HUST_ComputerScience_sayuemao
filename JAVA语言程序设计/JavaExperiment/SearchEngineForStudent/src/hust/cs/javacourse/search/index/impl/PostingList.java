package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractPostingList;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * PostingList is a concrete implementation of AbstractPostingList.
 * Contains a list of Posting objects for a term
 */
public class PostingList extends AbstractPostingList {

    @Override
    public void add(AbstractPosting posting) {
        if(posting instanceof Posting){
            if (!contains(posting)) {
                list.add(posting);
            }
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("{");
        for (int i = 0; i < list.size(); i++) {
            sb.append(list.get(i).toString());
            if (i < list.size() - 1) {
                sb.append(" -> ");
            }
        }
        sb.append('}');
        return sb.toString();
    }

    @Override
    public void add(List<AbstractPosting> postings) {
        for (AbstractPosting posting : postings) {
            add(posting);
        }
    }

    @Override
    public AbstractPosting get(int index) {
        return list.get(index);
    }

    @Override
    public int indexOf(AbstractPosting posting) {
        return list.indexOf(posting);
    }

    @Override
    public int indexOf(int docId) {
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getDocId() == docId) {
                return i;
            }
        }
        return -1;
    }

    @Override
    public boolean contains(AbstractPosting posting) {
        return list.contains(posting);
    }

    @Override
    public void remove(int index) {
        list.remove(index);
    }

    @Override
    public void remove(AbstractPosting posting) {
        list.remove(posting);
    }

    @Override
    public int size() {
        return list.size();
    }

    @Override
    public void clear() {
        list.clear();
    }

    @Override
    public boolean isEmpty() {
        return list.isEmpty();
    }

    @Override
    public void sort() {
        Collections.sort(list);
        //for (AbstractPosting posting : list) {
        //   posting.sort();
        //}
    }

    @Override
    public void writeObject(ObjectOutputStream out) {
        try {
            AbstractPosting[] postings = new AbstractPosting[list.size()];
            list.toArray(postings);
            out.writeObject(postings);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void readObject(ObjectInputStream in) {
        try {
            AbstractPosting[] postings = (AbstractPosting[])in.readObject();
            list =  Arrays.asList(postings);
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        PostingList other = (PostingList) obj;
        if (list.size() != other.list.size()) {
            return false;
        }
        for (AbstractPosting posting : list) {
            if (!other.contains(posting)) {
                return false;
            }
        }
        return true;
    }
}
