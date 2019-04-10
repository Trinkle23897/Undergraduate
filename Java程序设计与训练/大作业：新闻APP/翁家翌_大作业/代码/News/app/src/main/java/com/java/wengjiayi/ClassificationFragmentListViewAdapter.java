package com.java.wengjiayi;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ClassificationFragmentListViewAdapter extends BaseAdapter {

    Context context;
    String[] list;

    private int selectedPosition = -1;
    public void setSelectedPosition(int position) {
        selectedPosition = position;
    }

    public ClassificationFragmentListViewAdapter(Context context, String[] list) {
        this.context = context;
        this.list = list;
    }

    /*
     *listView中显示的数据个数
     */

    @Override
    public int getCount() {
        return list.length;
    }

    /*
     *根据position获取一个listView中一个item对象
     */
    @Override
    public Object getItem(int position) {
        return list[position];
    }

    /*
     *根据position获取一个listView中一个item位置
     */
    @Override
    public long getItemId(int position) {
        return position;
    }

    /*
     *绘制视图--1.创建视图，2.初始化控件，3.填充数据
     */
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        //第一步 创建视图 建一个XML文件
        //实例化View对象
        View view = LayoutInflater.from(context).inflate(R.layout.activity_show_description, null);
        //第二步
        //初始化控件
        TextView tvContent = (TextView) view.findViewById(R.id.all);
        //第三步 先获取数据
        //通过position获取要填充的数据（list的item的值）
        String contrnt =list[position];
        //设置选中效果
        if (selectedPosition == position){
            tvContent.setTextColor(Color.RED);
        }else {
            tvContent.setTextColor(Color.BLACK);
        }
        //将数据显示在控件上
        tvContent.setText(contrnt);
        return view;
    }

}
