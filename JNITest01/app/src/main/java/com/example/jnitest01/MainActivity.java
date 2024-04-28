package com.example.jnitest01;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    private String name = "xiao";
    private static int age = 20;
    private double number1 = 100.0;
    private final double number2 = 200.0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        System.out.println("修改前：" + name);
        changeName();
        System.out.println("修改后：" + name);

        System.out.println("修改前：" + age);
        changeAge();
        System.out.println("修改后：" + age);

        System.out.println("修改前：" + number1);
        System.out.println("修改前：" + number2);
        changeNumber();
        System.out.println("修改后：" + number1);
        System.out.println("修改后：" + number2);
        int r = callIntMethod();
        System.out.println("callIntMethod result：" + r);

        String str =  callStringMethod("牛逼");
        System.out.println("callStringMethod result：" + str);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void changeName();
    public static native void changeAge();
    public native void changeNumber();
    public native int callIntMethod();
    public int add(int num1, int num2){
        System.out.println("C调用了add函数");
        return num1 + num1;
    }
    public native String callStringMethod(String str);
    public String show(String str){
        System.out.println("C调用了show函数 " + str);
        return "[" + str + "]";
    }

    public native void testArray(int count, String str, int[] ints, String[] strs);
    public void test01(View view){
        int[] ints = new int[]{1,2,3,4,5,6};
        String[] strs = new String[]{"li","xiao","zhang"};
        testArray(99,"xiao",ints,strs);
        for (int i:ints){
            System.out.println("ints: " + i);
        }
        for (String i:strs){
            System.out.println("strs: " + i);
        }
    }

    public native void pubObject(Student student, String str);
    public void test02(View view){
        Student stu = new Student();
        stu.name = "史泰龙";
        stu.age = 88;
        pubObject(stu,"九阳神功");
        System.out.println("studnet:" + stu.toString());
    }
}
