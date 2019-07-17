package com.eighteengray.asmplugin;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;


public class AsmClassVisitor extends ClassVisitor implements Opcodes {

    private String mClassName;

    public AsmClassVisitor(ClassVisitor cv) {
        super(Opcodes.ASM5, cv);
    }

    @Override
    public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
        System.out.println("AsmClassVisitor : visit -----> started ：" + name);
        this.mClassName = name;
        super.visit(version, access, name, signature, superName, interfaces);
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
        System.out.println("AsmClassVisitor : visitMethod : " + name);
        MethodVisitor mv = cv.visitMethod(access, name, desc, signature, exceptions);
        //匹配FragmentActivity
        if ("android/support/v4/app/FragmentActivity".equals(this.mClassName)) {
            if ("onCreate".equals(name) ) {
                //处理onCreate
                System.out.println("AsmClassVisitor : change method ----> " + name);
                return new AsmOnCreateMethodVisitor(mv);
            } else if ("onDestroy".equals(name)) {
                //处理onDestroy
                System.out.println("AsmClassVisitor : change method ----> " + name);
                return new AsmOnDestroyMethodVisitor(mv);
            }
        }
        return mv;
    }

    @Override
    public void visitEnd() {
        System.out.println("AsmClassVisitor : visit -----> end");
        super.visitEnd();
    }
}
