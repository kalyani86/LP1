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
      limit.acquire();
      pro.acquire();
      this.item=item;
      buffer.add(item);
      System.out.println("Producer produced item no : "+item);
      System.out.println("Producer produced items : "+buffer.size());
      System.out.println("Producer can still produced items : "+limit.availablePermits());
      System.out.println();
    }
    catch(Exception e)
    {
      System.out.println("Exception caught !");
    }
    pro.release();
    con.release();
  }

  void get()
  {
    try
    {
      con.acquire();
      pro.acquire();
      if(buffer.size()>0)
      {
        System.out.println("Consumer Consumed : "+buffer.size()+" products");
        System.out.println("Number of items left : "+pro.availablePermits());  
        System.out.println();
        buffer.clear();
      }
    }
    catch(Exception e)
    {
      System.out.println("Exception caught !");
    }
    pro.release();
    for(int i=0;i<buffer.size();i++)
    {
      limit.release();
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
        Thread.sleep(5);
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
        Thread.sleep(5);
      }
    }
    catch(InterruptedException eobj)
    {
      System.out.println("Exception caught: " + eobj);
    }
  }
}

public class pc2 {
  public static void main(String args[]) {
    Container contain = new Container();
    new Producer(contain);
    new Consumer(contain);
}  
}
