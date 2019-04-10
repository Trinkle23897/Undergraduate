package com.java.wengjiayi;

import android.content.Context;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.widget.Toast;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import static android.content.Context.MODE_PRIVATE;

public class ShowCheckBox {
    ArrayList<Integer> yourChoices = new ArrayList<>();
    public void showMultiChoiceDialog(final Context context) {
        final String[] items = { "国内新闻","国际新闻","经济新闻","体育新闻","台湾新闻","教育新闻","文化新闻","游戏新闻" };
        // 设置默认选中的选项，全为false默认均未选中
        final boolean initChoiceSets[]={true,true,true,true,true,true,true,true};//true,true,true,true,true,true,true,true
        yourChoices.clear();
        for (int i = 0; i < items.length; ++i)
            yourChoices.add(i);
        AlertDialog.Builder multiChoiceDialog =
                new AlertDialog.Builder(context);
        multiChoiceDialog.setTitle("请选择要订阅的新闻类型");
        multiChoiceDialog.setMultiChoiceItems(items, initChoiceSets,
                new DialogInterface.OnMultiChoiceClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which,
                                        boolean isChecked) {
                        Log.i("choice", " "+which);
                        if (isChecked) {
                            yourChoices.add(which);
                        } else {
                            yourChoices.remove(which);
                        }
                    }
                });
        multiChoiceDialog.setPositiveButton("确定",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        int size = yourChoices.size();
                        String str = "";
                        String file = "";
                        for (int i = 0; i < size; i++) {
                            str += items[yourChoices.get(i)] + " ";
                            file += i + " ";
                        }
                        Toast.makeText(context,
                                "你选中了：" + str,
                                Toast.LENGTH_SHORT).show();
                        try {
                            FileOutputStream out = context.openFileOutput("9bf6d3581228", MODE_PRIVATE);
                            out.write(file.getBytes());
                            out.close();
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
        multiChoiceDialog.show();
    }
}
