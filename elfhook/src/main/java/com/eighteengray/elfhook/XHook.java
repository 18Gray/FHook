package com.eighteengray.elfhook;

import android.content.Context;
import android.util.Log;


/**
 * 读 maps，获取 ELF 的内存首地址（start address）。
 * 验证 ELF 头信息。
 * 从 PHT 中找到类型为 PT_LOAD 且 offset 为 0 的 segment。计算 ELF 基地址。
 * 从 PHT 中找到类型为 PT_DYNAMIC 的 segment，从中获取到 .dynamic section，从 .dynamic section中获取其他各项 section 对应的内存地址。
 * 在 .dynstr section 中找到需要 hook 的 symbol 对应的 index 值。
 * 遍历所有的 .relxxx section（重定位 section），查找 symbol index 和 symbol type 都匹配的项，对于这项重定位项，执行 hook 操作。hook 流程如下：
 * 读 maps，确认当前 hook 地址的内存访问权限。
 * 如果权限不是可读也可写，则用 mprotect 修改访问权限为可读也可写。
 * 如果调用方需要，就保留 hook 地址当前的值，用于返回。
 * 将 hook 地址的值替换为新的值。（执行 hook）
 * 如果之前用 mprotect 修改过内存访问权限，现在还原到之前的权限。
 * 清除 hook 地址所在内存页的处理器指令缓存。
 */
public class XHook {
    private static final XHook ourInstance = new XHook();
    private static boolean inited = false;

    public static XHook getInstance() {
        return ourInstance;
    }

    private XHook() {
    }

    public synchronized boolean isInited() {
        return inited;
    }

    public synchronized boolean init(Context ctx) {
        if(inited) {
            return true;
        }

        try {
            System.loadLibrary("xhook");
            inited = true;
        } catch (Throwable e) {
            try {
                System.load(ctx.getFilesDir().getParent() + "/lib/libxhook.so");
                inited = true;
            } catch (Throwable ex) {
                ex.printStackTrace();
                Log.e("xhook", "load libxhook.so failed");
            }
        }
        return inited;
    }


    public native int refresh(boolean async);

    public native void clear();

    public native void enableDebug(boolean flag);

    public native void enableSigSegvProtection(boolean flag);

}
