#include  <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/vmstat.h> 


MODULE_LICENSE("GPL"); //*
MODULE_AUTHOR("MWCSUR"); //!
MODULE_DESCRIPTION("Memory reporting module!");//!

#define TIMEOUT 5000

//setup timer for periodic activity
int x=0;

static unsigned long total_space=0;
static unsigned long space_left=0;

//setup proc file stuff
static struct proc_dir_entry *proc_entry;

//proc file read method
static ssize_t memmod_read(struct file* file_pointer,
                char *user_space_buffer,
                size_t count,
                loff_t* offset){

        
    struct sysinfo info;

    // Get memory information
    si_meminfo(&info);

    // Calculate total and free memory in bytes, KB, MB, GB
    unsigned long free_ram = ((info.freeram * info.mem_unit)/1024)/1024;


    unsigned long total_bytes= totalram_pages() * PAGE_SIZE;
    unsigned long total_kb = total_bytes / 1024;
    unsigned long total_ram = total_kb / 1024;


    total_space = total_ram;
    space_left = free_ram;

        // //write to user space
        char msg[255];
        char msg2[]="space read!\n";
        sprintf(msg,"Total Ram: %ld. Free Ram Left: %ld\n",total_space,space_left);
        unsigned int len = strlen(msg);
        int res2;
        res2=copy_to_user(user_space_buffer,msg,len);
        if(*offset>=len){
            return 0;
        }
        *offset+=len;
        return len;
}

//proc
static struct proc_ops memmod_proc_ops={
    .proc_read=memmod_read
};


static int mem_module_init(void){
    printk(KERN_INFO "mem_module init: start ....\n");

    //create proc entries
    proc_entry = proc_create(
        "mem_proc_driver",
        0,
        NULL,
        &memmod_proc_ops
    );

    if (proc_entry == NULL) {
    printk(KERN_ERR "memmod: Failed to create /proc entry\n");
    return -ENOMEM;
    }

    // //timer setup
    // timer_setup(&mem_timer,mem_timer_callback,0);
    // mod_timer(&mem_timer,jiffies+msecs_to_jiffies(TIMEOUT));

    printk(KERN_INFO "mem_module init: finished....\n");
    return 0;
}

static void mem_module_exit(void){
    printk(KERN_INFO "mem module cleanup: start.....\n");

    // del_timer(&mem_timer);

    proc_remove(proc_entry);


    printk(KERN_INFO "mem module cleanup: finished.....\n");
}


module_init(mem_module_init);
module_exit(mem_module_exit);