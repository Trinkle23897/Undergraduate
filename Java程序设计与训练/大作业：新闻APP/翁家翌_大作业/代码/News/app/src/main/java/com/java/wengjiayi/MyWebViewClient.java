package com.java.wengjiayi;

import android.util.Log;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class MyWebViewClient extends WebViewClient {
    String baseUrl;
    MyWebViewClient(String url) { baseUrl = url; }
    @Override
    public boolean shouldOverrideUrlLoading(WebView view, String url) { //网页加载时的连接的网址
        if (!url.startsWith("http:") ) {
            view.loadUrl(baseUrl+url);
            Log.i("imgurl", baseUrl+url);
            return false;
        }
        return true;
    }
}
