package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractPosting;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Posting is a concrete implementation of AbstractPosting.
 * Represents an entry in the inverted index, containing docId, frequency, and
 * positions
 */
public class Posting extends AbstractPosting {

    public Posting() {
    }

    public Posting(int docId, int freq, List<Integer> positions) {
         super(docId, freq, positions);
    }

    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Posting){
            Posting p = (Posting) obj;
            this.sort();
            p.sort();
            return this.docId == p.docId && this.freq == p.freq && this.positions.equals(p.positions);
        }
        return false;
    }

    @Override
    public String toString() {
        return "\"docId\":" + docId +
                ", \"freq\":" + freq +
                ", \"positions\":" + positions
                ;
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
    public int getFreq() {
        return freq;
    }

    @Override
    public void setFreq(int freq) {
        this.freq = freq;
    }

    @Override
    public List<Integer> getPositions() {
        return positions;
    }

    @Override
    public void setPositions(List<Integer> positions) {
        this.positions = positions ;
    }

    @Override
    public int compareTo(AbstractPosting o) {
        if(o instanceof Posting) {
            return this.docId - o.getDocId();
        }
        return 0;
    }

    @Override
    public void sort() {
        Collections.sort(positions);
    }

    @Override
    public void writeObject(ObjectOutputStream out) {
        try {
            out.writeObject(docId);
            out.writeObject(freq);
            Integer[] integers = new Integer[positions.size()];
            positions.toArray(integers);
            out.writeObject(integers);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void readObject(ObjectInputStream in) {
        try {
            docId = (int) in.readObject();
            freq = (int) in.readObject();
            Integer[] integers = (Integer[]) in.readObject();
            positions = new ArrayList<>(Arrays.asList(integers));
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}
