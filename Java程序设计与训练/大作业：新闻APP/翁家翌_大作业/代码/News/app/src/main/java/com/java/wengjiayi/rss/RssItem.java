package com.java.wengjiayi.rss;


import android.util.Log;

import com.java.wengjiayi.MainActivity;

import java.io.File;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RssItem {

    private String title;
    private String description;
    private String link;
    private String category;
    private String pubdate;

    public static final String TITLE = "title";
    public static final String PUBDATE = "pubdate";
    public static final String DESC = "description";

    public RssItem() {

    }
    public RssItem(String q) {
        title = q;
        description = q;
        pubdate = q;
    }

    public String getTitle() {
//        System.out.println(title);
        if (title.length() > 20) {
            return title.substring(0, 19) + "...";
        }
        return title;
    }

    public void setTitle(String title) { this.title = title; }

    public String getDescription() {
        if (description == null)
            return "[ 点击阅读更多 ]";
        Log.i("length"," "+description.length());
        if (description.length() > 23) {
            return description.substring(0, 22) + "...";
        }
        return description;
    }

    public void setDescription(String description) {
        description = description.replaceAll("<(.*?)>", " ").replaceAll("&nbsp;", "").replaceAll(" {2,}", " ") ;
        this.description = description;
//        Log.i("desc", description);
    }

    public String getLink() {
        return link;
    }

    public void setLink(String link) {
        this.link = link;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }

    public String getPubdate() {
        return pubdate;
    }

    public void setPubdate(String pubdate) {
        this.pubdate = pubdate;
    }

    @Override
    public String toString() {
        return "RssItem [title=" + title + ", description=" + description
                + ", link=" + link + ", category=" + category + ", pubdate="
                + pubdate + "]";
    }

}
