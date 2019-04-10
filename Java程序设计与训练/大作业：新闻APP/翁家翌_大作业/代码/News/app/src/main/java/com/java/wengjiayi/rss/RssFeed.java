 package com.java.wengjiayi.rss;

import com.java.wengjiayi.MainActivity;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

public class RssFeed {

    private String title; // 标题
    private String pubdate; // 发布日期
    private int itemCount; // 用于计算列表的数目
    private List<RssItem> rssItems; // 用于描述列表item
    public List<HashMap<String, Object>> data;
    public RssFeed() {
        rssItems = new ArrayList<RssItem>();
        data = new ArrayList<HashMap<String, Object>>();
    }

    // 添加RssItem条目,返回列表长度
    public int addItem(RssItem rssItem) {
        rssItems.add(rssItem);
        itemCount++;
        return itemCount;
    }

    // 根据下标获取RssItem
    public RssItem getItem(int position) {
        return rssItems.get(position);
    }

    public void refresh() {
        Collections.shuffle(rssItems);
    }
    public List<HashMap<String, Object>> getAllItems(String path) {
        data.clear();
        for (int i = 0; i < rssItems.size(); i++) {
            File file = new File(path+"/"+rssItems.get(i).getLink().replace("/", "_").replace(":", ""));
            if (file.exists() && !rssItems.get(i).getTitle().startsWith("[已读] "))
                rssItems.get(i).setTitle("[已读] " + rssItems.get(i).getTitle());
            HashMap<String, Object> item = new HashMap<String, Object>();
            item.put(RssItem.TITLE, rssItems.get(i).getTitle());
            item.put(RssItem.PUBDATE, rssItems.get(i).getDescription());
            data.add(item);
        }
        return data;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getPubdate() {
        return pubdate;
    }

    public void setPubdate(String pubdate) {
        this.pubdate = pubdate;
    }

    public int getItemCount() {
        return itemCount;
    }

    public void setItemCount(int itemCount) {
        this.itemCount = itemCount;
    }

    public ArrayList<String> getLinks() {
        ArrayList<String> links = new ArrayList<String>();
        for (int i = 0; i < rssItems.size(); ++i)
            links.add(rssItems.get(i).getLink());
        return links;
    }
}