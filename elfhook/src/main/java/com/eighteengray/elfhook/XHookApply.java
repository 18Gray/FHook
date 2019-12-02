package com.eighteengray.elfhook;

public class XHookApply {

    private static final XHookApply ourInstance = new XHookApply();

    public static XHookApply getInstance() {
        return ourInstance;
    }

    private XHookApply() {
    }

    public synchronized void init() {
        System.loadLibrary("xhookapply");
    }


    public native void start();
}

