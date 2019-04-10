package com.java.wengjiayi;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

//import com.handmark.pulltorefresh.library.PullToRefreshBase;
//import com.handmark.pulltorefresh.library.PullToRefreshListView;
import com.java.wengjiayi.rss.*;
import com.jwenfeng.library.pulltorefresh.BaseRefreshListener;
import com.jwenfeng.library.pulltorefresh.PullToRefreshLayout;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.design.widget.NavigationView;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.support.v4.app.NavUtils;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements OnItemClickListener{//, NavigationView.OnNavigationItemSelectedListener {

    // 从网络获取RSS地址
    public String RSS_URL = "http://news.qq.com/newssh/rss_newssh.xml";
            // "http://www.people.com.cn/rss/politics.xml";
            // "http://www.people.com.cn/rss/world.xml";

    public final String tag = "RSSReader";
    private RssFeed feed = null;
    private ListView newsList;
    private SimpleAdapter simpleAdapter;
    boolean choiceSets[] = {true,true,true,true,true,true,true,true};
    Context context;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        context = this;
        // 获取控件

//        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
//        setSupportActionBar(toolbar);

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });

//        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
//        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
//                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
//        drawer.addDrawerListener(toggle);
//        toggle.syncState();

//        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
//        navigationView.setNavigationItemSelectedListener(this);
        RSS_URL = "http://www.people.com.cn/rss/world.xml";
//        TextView tv = (TextView)findViewById(R.id.title);
//        tv.setText("国际新闻");
        refreshMain();
        String prefix = getFilesDir().getPath();
        File file = new File(prefix+"/9bf6d3581229");
        String state = "11111111";
        if (file.exists()) {
            try {
                FileInputStream inStream=this.openFileInput("9bf6d3581229");
                ByteArrayOutputStream stream=new ByteArrayOutputStream();
                byte[] buffer=new byte[8];
                int length=-1;
                if((length=inStream.read(buffer))!=-1)
                    stream.write(buffer,0,length);
                stream.close();
                inStream.close();
                state = stream.toString();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            catch (IOException e){
                e.printStackTrace();
            }
        }
        Log.i("init", state);
        setUpColumn(R.id.col1, "http://www.people.com.cn/rss/politics.xml");
        setUpColumn(R.id.col2, "http://www.people.com.cn/rss/world.xml");
        setUpColumn(R.id.col3, "http://www.people.com.cn/rss/finance.xml");
        setUpColumn(R.id.col4, "http://www.people.com.cn/rss/sports.xml");
        setUpColumn(R.id.col5, "http://www.people.com.cn/rss/haixia.xml");
        setUpColumn(R.id.col6, "http://www.people.com.cn/rss/edu.xml");
        setUpColumn(R.id.col7, "http://www.people.com.cn/rss/culture.xml");
        setUpColumn(R.id.col8, "http://www.people.com.cn/rss/game.xml");
        setState(state);
        setPullToRefesh();
    }
    private void setState(String state) {
        TextView tv;
        Log.i("state", state);
        for (int i = 0; i < 8; ++i) {
            int id;
            if (i == 0) id = R.id.col1;
            else if (i == 1) id = R.id.col2;
            else if (i == 2) id = R.id.col3;
            else if (i == 3) id = R.id.col4;
            else if (i == 4) id = R.id.col5;
            else if (i == 5) id = R.id.col6;
            else if (i == 6) id = R.id.col7;
            else id = R.id.col8;
            tv = (TextView) findViewById(id);
            if (state.charAt(i) == '1') {
                tv.setVisibility(View.VISIBLE);
                Log.i("state", "true");
                choiceSets[i] = true;
            }
            else {
                tv.setVisibility(View.GONE);
                Log.i("state", "false");
                choiceSets[i] = false;
            }
        }
    }
    private void refreshMain() {
        Log.i("rssurl:", RSS_URL);
        Thread t = new Thread(new Runnable(){
            @Override
            public void run() {
                try {
                    feed = new RssFeed_SAXParser().getFeed(RSS_URL);
                } catch (ParserConfigurationException e) {
                    e.printStackTrace();
                } catch (SAXException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        t.start();
        try{
            t.join();
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
        showListView();
    }
    public void showMultiChoiceDialog(final Context context) {
        final ArrayList<Integer> yourChoices = new ArrayList<>();
        final String[] items = { "国内新闻","国际新闻","经济新闻","体育新闻","台湾新闻","教育新闻","文化新闻","游戏新闻" };
        // 设置默认选中的选项，全为false默认均未选中
//        yourChoices.clear();
//        for (int i = 0; i < items.length; ++i)
//            if (choiceSets[i])
//                yourChoices.add(i);
//        System.out.println(yourChoices);
        android.support.v7.app.AlertDialog.Builder multiChoiceDialog =
                new android.support.v7.app.AlertDialog.Builder(context);
        multiChoiceDialog.setTitle("请选择要订阅的新闻类型");
        multiChoiceDialog.setMultiChoiceItems(items, choiceSets,
                new DialogInterface.OnMultiChoiceClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which,
                                        boolean isChecked) {
                        Log.i("choice", " "+which);
                        if (isChecked) {
                            choiceSets[which] = true;
//                            yourChoices.add(which);
                        } else {
                            choiceSets[which] = false;
                        }
                    }
                });
        multiChoiceDialog.setPositiveButton("确定",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
//                        int size = yourChoices.size();
//                        boolean[] choice = {false,false,false,false,false,false,false,false};
                        String str = "";
                        String file = "";
                        for (int i = 0; i < choiceSets.length; ++i) {
                            if (choiceSets[i]) {
                                str += items[i] + " ";
                                file += "1";
                            }
                            else
                                file += "0";
                        }
                        Toast.makeText(context,
                                "你选中了：" + str,
                                Toast.LENGTH_SHORT).show();
                        try {
                            FileOutputStream out = context.openFileOutput("9bf6d3581229", MODE_PRIVATE);
                            out.write(file.getBytes());
                            out.close();
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        setState(file);
                    }
                });
        multiChoiceDialog.show();
    }
    private void setPullToRefesh(){
        final PullToRefreshLayout pullToRefreshLayout = (PullToRefreshLayout)findViewById(R.id.id_lv_up);
        pullToRefreshLayout.setRefreshListener(new BaseRefreshListener() {
            @Override
            public void refresh() {
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        Thread t = new Thread(new Runnable(){
                            @Override
                            public void run() {
                                try {
                                    feed = new RssFeed_SAXParser().getFeed(RSS_URL);
                                } catch (ParserConfigurationException e) {
                                    e.printStackTrace();
                                } catch (SAXException e) {
                                    e.printStackTrace();
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }
                        });
                        t.start();
                        try{
                            t.join();
                        }
                        catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        showListView();
                        Toast.makeText(context, "没有更多新闻了哟～", Toast.LENGTH_SHORT).show();
//                        feed.refresh();
                        // 结束刷新
                        pullToRefreshLayout.finishRefresh();
                    }
                }, 1000);

            }

            @Override
            public void loadMore() {
                new Handler().postDelayed(new Runnable() {

                    @Override
                    public void run() {
                        feed.refresh();
                        simpleAdapter = new SimpleAdapter(context, feed.getAllItems(getFilesDir().toString()), android.R.layout.simple_list_item_2, new String[] { RssItem.TITLE, RssItem.PUBDATE,  }, new int[]{android.R.id.text1, android.R.id.text2});
                        newsList.setAdapter(simpleAdapter);
                        newsList.setSelection(newsList.getHeaderViewsCount());
//                        noMoreFlag = true;
                        // 结束刷新
                        pullToRefreshLayout.finishLoadMore();
                        Toast.makeText(context, "又加载了好多呢～", Toast.LENGTH_SHORT).show();
                    }
                }, 1000);
            }
        });
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_share) {
            showMultiChoiceDialog(context);
//            Toast.makeText(context, "要分享哪个新闻呢？", Toast.LENGTH_SHORT).show();
            return true;
        }
        if (id == R.id.action_clear) {
            Toast.makeText(context, "缓存清空成功！", Toast.LENGTH_SHORT).show();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /*
     * 把RSS内容绑定到ui界面进行显示
     */
    private void setUpColumn(int id, final String url) {
        TextView tv = (TextView)findViewById(id);
        final String s = tv.getText().toString();
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("click!", url);
//                Toast.makeText(context,"获取"+s+"中", Toast.LENGTH_SHORT).show();
                RSS_URL = url;
                refreshMain();
                Toast.makeText(context,s+"切换成功!\n向右滑动返回", Toast.LENGTH_SHORT).show();
            }
        });
    }
    private void showListView() {
        Log.i(tag,"success");
        newsList = (ListView) this.findViewById(R.id.list);
        if (!NetworkAvail.check(this)) {
            ShowDescriptionActivity.show("没有网络qwq请检查", this, true);
            Log.i("network", "no");
            return;
        }
        else
            Log.i("network", "yes ");
        if (feed == null) {
            ShowDescriptionActivity.show("访问的RSS无效", this, true);
            return;
        }

        simpleAdapter = new SimpleAdapter(this,
                feed.getAllItems(getFilesDir().toString()), android.R.layout.simple_list_item_2,
                new String[] { RssItem.TITLE, RssItem.PUBDATE,  }, new int[] {
                android.R.id.text1, android.R.id.text2 });
        newsList.setAdapter(simpleAdapter);
        newsList.setOnItemClickListener(this);
        newsList.setSelection(0);
    }
//    @Override
//    public void onBackPressed() {
//        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
//        if (drawer.isDrawerOpen(GravityCompat.START)) {
//            drawer.closeDrawer(GravityCompat.START);
//        } else {
//            super.onBackPressed();
//        }
//    }

//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        getMenuInflater().inflate(R.menu.main, menu);
//        return true;
//    }

//    @Nullable
//    @Override
//    public View onCreateView(String name, Context context, AttributeSet attrs) {
//        return super.onCreateView(name, context, attrs);
//    }

    @Override
    public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
        String s = feed.getItem(position).getTitle();

        Intent intent = new Intent();
        intent.setClass(this, ItemNewsActivity.class);
        intent.putExtra("link", feed.getItem(position).getLink());
        Bundle bundle = new Bundle();
        bundle.putString("title", feed.getItem(position).getTitle());
        bundle.putString("description",feed.getItem(position).getDescription());
        bundle.putString("link", feed.getItem(position).getLink());
        bundle.putString("pubdate", feed.getItem(position).getPubdate());
        // 用android.intent.extra.INTENT的名字来传递参数
        intent.putExtra("android.intent.extra.rssItem", bundle);
        startActivity(intent);
        int first = newsList.getFirstVisiblePosition();
        if (!s.startsWith("[已读] ")) {
            feed.getItem(position).setTitle("[已读] " + s);
            simpleAdapter = new SimpleAdapter(this,
                    feed.getAllItems(getFilesDir().toString()), android.R.layout.simple_list_item_2,
                    new String[] { RssItem.TITLE, RssItem.PUBDATE,  }, new int[] {
                    android.R.id.text1, android.R.id.text2 });
//            newsList.getChildAt(position-first).setBackgroundColor(getResources().getColor(R.color.colorAccent));
            newsList.setAdapter(simpleAdapter);
            newsList.setSelection(first);
        }
    }
//    @SuppressWarnings("StatementWithEmptyBody")
//    @Override
//    public boolean onNavigationItemSelected(MenuItem item) {
//        // Handle navigation view item clicks here.
//        int id = item.getItemId();
//
//        if (id == R.id.nav_camera) {
//            // Handle the camera action
//        } else if (id == R.id.nav_gallery) {
//
//        } else if (id == R.id.nav_slideshow) {
//
//        } else if (id == R.id.nav_manage) {
//
//        } else if (id == R.id.nav_share) {
//
//        } else if (id == R.id.nav_send) {
//
//        }
//
//        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
//        drawer.closeDrawer(GravityCompat.START);
//        return true;
//    }
}
