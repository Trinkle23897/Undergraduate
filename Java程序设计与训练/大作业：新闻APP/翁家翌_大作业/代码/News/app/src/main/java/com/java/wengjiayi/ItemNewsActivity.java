package com.java.wengjiayi;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Intent;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;

public class ItemNewsActivity extends AppCompatActivity {

    public String webUrl;
    private WebView webView;
    public String html;
    public int networkState;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_newstop_content);
        Intent intent = getIntent();

        webUrl = intent.getStringExtra("link");
        setupViews();
        showContent();
    }

    private void setupViews() {
//        scrollView = (ScrollView) findViewById(R.id.news_scrollView);
//        newsContentView = (NewsContentView) findViewById(R.id.newscontentView);
        webView = (WebView) findViewById(R.id.news_webView);
        webView.getSettings().setDomStorageEnabled(true);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.getSettings().setAppCacheEnabled(true);
        webView.getSettings().setSupportZoom(true);
        webView.getSettings().setBuiltInZoomControls(true);
        webView.setWebChromeClient(new WebChromeClient());
        webView.setWebViewClient(new MyWebViewClient("http://world.people.com.cn"));
        webView.getSettings().setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);
        webView.getSettings().setLayoutAlgorithm(WebSettings.LayoutAlgorithm.SINGLE_COLUMN);
        webView.getSettings().setLoadWithOverviewMode(true);
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.item, menu);
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
            Intent share = new Intent();
            share.setAction(Intent.ACTION_SEND);
            share.setType("text/html");
//            share.putExtra(Intent.EXTRA_SUBJECT, "233");
            share.putExtra(Intent.EXTRA_TEXT, WebContent.getTitle(html).replace("&nbsp;","")+" "+webUrl);
            share = Intent.createChooser(share, "分享该新闻至：");
            startActivity(share);
            return true;
        }
        if (id == R.id.action_clear) {
            Toast.makeText(this, "缓存清空成功！", Toast.LENGTH_SHORT).show();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void showContent() {
        if (webUrl.indexOf("qq.com") > -1)
            webUrl = webUrl.replace("http", "https");
        Log.i("loadurl", webUrl);
        String fileName = webUrl.replace("/", "_").replace(":", "");
        String prefix = getFilesDir().getPath();
        File file = new File(prefix+"/"+fileName);
        Log.i("loadurl", prefix+"/"+fileName);
        networkState = 1;
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    html = WebContent.getHtml(webUrl);
                } catch (ExceptionInInitializerError e) {
                    networkState = 0;
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
        if (!file.exists() || networkState == 1) {
            Log.i("state", "no exist " + networkState);
            if (networkState == 1) {
                String charSet = "";
                try {
                    charSet = WebContent.getCharset(html);
                } catch (ExceptionInInitializerError e) {
                    ShowDescriptionActivity.show("网络断掉了qwq", this, false);
                }
                webView.loadDataWithBaseURL(null, html, "text/html", charSet, null);
                webView.loadUrl(webUrl);
            }
            else
                html = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta http-equiv=\"content-type\" content=\"text/html;charset=GB2312\"/><meta http-equiv=\"Content-Language\" content=\"utf-8\" /><meta content=\"all\" name=\"robots\" /><title>网络断掉了qwq</title><html><h1>网络断掉了qwq</h1><h5>请检查网络连接。</h5></html>";
//                ShowDescriptionActivity.show("网络断掉了qwq", this, false);
            if (networkState == 1) {
                Log.i("html", html);
                try {
                    FileOutputStream out = openFileOutput(fileName, MODE_PRIVATE);
                    out.write(html.getBytes());
                    out.close();
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else {
                webView.loadDataWithBaseURL(null, html, "text/html", "GB2312", null);
            }
        }
        else {
            Log.i("state", "exist");
            try {
                FileInputStream inStream=this.openFileInput(fileName);
                ByteArrayOutputStream stream=new ByteArrayOutputStream();
                byte[] buffer=new byte[1024];
                int length=-1;
                while((length=inStream.read(buffer))!=-1)   {
                    stream.write(buffer,0,length);
                }
                stream.close();
                inStream.close();
                html = stream.toString();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            catch (IOException e){
                e.printStackTrace();
            }
            String charSet = "";
            try {
                charSet = WebContent.getCharset(html);
            } catch (ExceptionInInitializerError e) {
                ShowDescriptionActivity.show("网络断掉了qwq", this, false);
            }
            webView.loadDataWithBaseURL(null, html, "text/html", charSet, null);
        }
    }

    //点击backspace可返回上个页面，而不是退出(若webview只加载了一个页面)
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (webView.getVisibility() == View.VISIBLE) {
                // 按返回时，看网页是否能返回
                if (webView.canGoBack()) {
                    webView.goBack();
                    //返回true webview自己处理
                    return true;
                }
            }
        }
        return super.onKeyDown(keyCode, event);
    }
}
