import java.util.*;
import java.util.concurrent.Semaphore;

class Container
{
  int item;
  Vector<Integer> buffer=new Vector<Integer>(100);
  static Semaphore con=new Semaphore(0);
  static Semaphore pro=new Semaphore(1);
  static Semaphore limit=new Semaphore(100);

  void put(int item)
  {
    try
    {
      pro.acquire();
      limit.acquire();
      this.item=item;
      buffer.add(item);
      System.out.println("Producer produced item no : "+item);
      System.out.println();
      pro.release();
      con.release();
    }
    catch(Exception e)
    {
      System.out.println("Exception caught !");
    }
  }

  void get()
  {
    try
    {
      pro.acquire();
      con.acquire();
      if(buffer.size()>0)
      {
        System.out.println("Consumer Consumed : "+buffer.firstElement()); 
        System.out.println();
      }
      buffer.remove(0);
      limit.release();
      pro.release();
    }
    catch(Exception e)
    {
      System.out.println("Exception caught !");
    }  
  }
}

class Consumer implements Runnable
{
  Container ccontain;
  Thread t2;

  Consumer(Container c)
  {
    ccontain=c;
    t2=new Thread(this,"consumer");
    t2.start();
  }

  public void run()
  {
    try
    {
      for(int i=1;i<=100;i++)
      {
        ccontain.get();
        Thread.sleep(6);
      }
    }
    catch(InterruptedException eobj)
    {
      System.out.println("Exception caught: " + eobj);
    }
  }
}

class Producer extends Thread
{
  Container pcontain;
  Thread t1;

  Producer(Container c)
  {
    pcontain=c;
    t1=new Thread(this,"producer");
    t1.start();
  }

  public void run()
  {
    try
    {
      for(int i=1;i<=100;i++)
      {
        pcontain.put(i);
        Thread.sleep(4);
      }
    }
    catch(InterruptedException eobj)
    {
      System.out.println("Exception caught: " + eobj);
    }
  }
}

public class pc3 {
  public static void main(String args[]) {
    Container contain = new Container();
    new Producer(contain);
    new Consumer(contain);
}  
}
