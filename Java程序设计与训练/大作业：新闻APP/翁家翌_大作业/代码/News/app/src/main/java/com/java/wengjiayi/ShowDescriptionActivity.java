package com.java.wengjiayi;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AlertDialog;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.support.v4.app.NavUtils;

public class ShowDescriptionActivity {
    public static void show(String e, Context c, final boolean exit) {
        AlertDialog.Builder d = new AlertDialog.Builder(c);
        d.setTitle(e);
        d.setPositiveButton("确定",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if (exit)
                            System.exit(0);
                    }
                });
        d.show();
    }
}
