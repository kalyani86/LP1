import java.util.*;
import java.util.concurrent.Semaphore;

class Container
{
  int item;
  Vector<Integer> buffer=new Vector<Integer>(10);
  static Semaphore con=new Semaphore(0);
  static Semaphore pro=new Semaphore(1);
  static Semaphore limit=new Semaphore(10);
  Random random=new Random();

  void put(int item)
  {
    try
    {
      pro.acquire();
      limit.acquire();
      this.item=item;
      buffer.add(item);
      System.out.println("\nProducer produced item no : "+item);
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
        int ridx=random.nextInt(buffer.size());
        int citem=buffer.remove(ridx);
        System.out.println("\nConsumer Consumed : "+citem); 
      }
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
      for(int i=1;i<=10;i++)
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
      for(int i=1;i<=10;i++)
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

public class pc4 {
  public static void main(String args[]) {
    Container contain = new Container();
    new Producer(contain);
    new Consumer(contain);
}  
}
