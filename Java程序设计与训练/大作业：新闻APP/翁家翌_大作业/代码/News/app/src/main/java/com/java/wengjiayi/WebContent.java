package com.java.wengjiayi;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class WebContent {

    /**
     * 返回当前URL的网页HTML代码
     *
     * @param htmlURL
     *            - 网页地址
     * @return 当前网页的HTML代码
     */
    static public String getHtml(String htmlURL) {
        URL url;
        Scanner in = null;
        StringBuilder sb = new StringBuilder();
        String charset;
        System.out.println(htmlURL);
        try {
            url = new URL(htmlURL);
            charset = getCharset(htmlURL);
            System.out.println(charset);
            in = new Scanner(new InputStreamReader(url.openStream(), charset));
            while (in.hasNextLine()) {
                sb.append(in.nextLine());
            }
            return sb.toString();
        } catch (MalformedURLException e) {
            System.out.println("URL错误！！"+sb.toString());
            throw new ExceptionInInitializerError(e);
        } catch (IOException e) {
            System.out.println("字符串处理出错！！"+sb.toString());
            throw new ExceptionInInitializerError(e);
        } finally {
            if (in != null) {
                in.close();
            }
        }

    }

    /**
     * 通过URL取得一个网页的编码集
     *
     * @param htmlURL
     *            - 网页URL
     * @return 编码集名称<br />
     *         如果未找到则返回 <b>空字符串</b>
     */
    static public String getCharset(final String htmlURL) {
        Scanner in = null;
        try {
            URL url = new URL(htmlURL);
            in = new Scanner(url.openStream());
            StringBuilder sb = new StringBuilder();
            String regex = "charset=[^>\\s]*\\b";
            Pattern p = Pattern.compile(regex, Pattern.CASE_INSENSITIVE);
            Matcher m;
            while (in.hasNextLine()) {
                sb.append(in.nextLine());
                m = p.matcher(sb.toString());
                if (m.find()) {
                    return m.group().toLowerCase()
                            .replaceAll("charset=\"?", "").trim();
                }
            }
            return "";
        } catch (MalformedURLException e) {
            return "gb2312";
//            System.out.println("URL错误！！");
//            throw new ExceptionInInitializerError(e);
        } catch (IOException e) {
            return "gb2312";
//            System.out.println("字符串处理出错！！");
//            throw new ExceptionInInitializerError(e);
        } finally {
            if (in != null) {
                in.close();
            }
        }
    }

    /**
     * 从网页HTML代码中去的文章标题
     *
     * @param html
     *            - 网页HTML代码
     * @return 文章标题<br />
     *         如果未找到则返回 <b>空字符串</b>
     */
    static public String getTitle(final String html) {
        String regex = "<title>[\\s\\S]*?</title>";
        Pattern p = Pattern.compile(regex, Pattern.CASE_INSENSITIVE);
        Matcher m = p.matcher(html);
        if (m.find()) {
            return m.group().toLowerCase().replaceAll("</?title>", "").trim();
        } else {
            return "";
        }
    }

    public static void write(String htmlURL) throws IOException {
//        String htmlURL = "http://www.baidu.com";
//        String htmlURL = "http://www.tangshanqiche.com/nNews/ShowNews.aspx?PageNumber=2&NewsClassTypeId=105";
        String html = getHtml(htmlURL);
//        String title = getTitle(html);

//        PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(
//                getFilesDir().toString() + "/" + htmlURL.replace("/", "_"))));
//        out.write(html);
//        out.close();
    }

}