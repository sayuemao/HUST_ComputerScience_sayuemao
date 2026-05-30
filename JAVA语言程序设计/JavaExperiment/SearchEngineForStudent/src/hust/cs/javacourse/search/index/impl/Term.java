package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.util.Config;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;

/**
 * Term is a concrete implementation of AbstractTerm.
 * Represents a word in text document
 */
public class Term extends AbstractTerm {

    public Term() {
    }

    public Term(String content) {
        this.content = content;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null || getClass() != obj.getClass())
            return false;
        Term term = (Term) obj;
        if (Config.IGNORE_CASE) {
            return content.equalsIgnoreCase(term.content);
        }
        return content.equals(term.content);
    }

    @Override
    public String toString() {
        return content;
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
    public int compareTo(AbstractTerm o) {
        if (Config.IGNORE_CASE) {
            return this.content.compareToIgnoreCase(o.getContent());
        }
        return this.content.compareTo(o.getContent());
    }

    @Override
    public void writeObject(ObjectOutputStream out) {
        try {
            out.writeObject(content);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void readObject(ObjectInputStream in) {
        try {
            content = (String) in.readObject();
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}
