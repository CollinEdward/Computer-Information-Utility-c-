// Color padllet used: https://coolors.co/1b264f-274690-576ca8-302b27-f5f3f5

// GUI and standard lib
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Multiline_Output.H>


// network and system lib
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>

// sys info lib
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>

using namespace std;

// just exit the program
void exit_button_callback(Fl_Widget* button, void* data) {
    // code to be executed when button is pressed
    // for example,
    exit(1);
}

void simple_info(){

    int width = 1200;
    int height = 800;
    
    Fl_Window* window2 = new Fl_Window(width, height, "Simple Information");


    Fl_Box *title = new Fl_Box(width/2-150, 80, 400, 80, "Simple information");
    title->box(_FL_PLASTIC_ROUND_UP_BOX);
    title->labelfont(FL_BOLD+FL_ITALIC);
    title->labelsize(36);
    title->labeltype(FL_SHADOW_LABEL);

    struct utsname sys_info;
    uname(&sys_info);

    // information in variables
    string sys_name = sys_info.sysname;
    string sys_release = sys_info.release;
    string sys_version = sys_info.version;
    
    uint64_t total_mem, free_mem;
    size_t len = sizeof(total_mem);
    sysctlbyname("hw.memsize", &total_mem, &len, NULL, 0);
    len = sizeof(free_mem);
    sysctlbyname("hw.usermem", &free_mem, &len, NULL, 0);

    int mem_total_gb = total_mem / (1024 * 1024 * 1024);
    int mem_free_gb = free_mem / (1024 * 1024 * 1024);


    string str_total_mem = to_string(mem_total_gb);
    string str_free_mem = to_string(mem_free_gb);


    cout << "Total Memory: " << mem_total_gb << " GB" << endl;
    cout << "Free Memory: " << mem_free_gb << " GB" << endl;

    int active_cpu;
    size_t len = sizeof(active_cpu);
    sysctlbyname("hw.activecpu", &active_cpu, &len, NULL, 0);
    cout << "Active CPU: " << active_cpu << endl;

    string str_active_cpu = to_string(active_cpu);

    Fl_Box *info_main = new Fl_Box(20, 180, width-30, 600);
    info_main->box(_FL_PLASTIC_ROUND_UP_BOX);
    info_main->labelfont(FL_BOLD+FL_ITALIC);
    info_main->labelsize(18);
    info_main->labeltype(FL_SHADOW_LABEL);
    info_main->copy_label(("System Name: " + sys_name + " \nSystem Version: " 
                             + sys_version + " \nSystem Release: " + sys_release + " \nTotal RAM: " 
                             + str_total_mem + " GB" +" \n Free RAM: " + str_free_mem + " GB"
                             + "Active CPU Usage: " + str_active_cpu).c_str());
    
    info_main->align(FL_ALIGN_CENTER);



    window2->show();
}

// button for the simple information info.
void simple_info_btn(Fl_Widget* button, void* data) {
    // code to be executed when button is pressed
    simple_info();
}

void network_info(){
    int width = 800; 
    int height = 800;
    Fl_Window* window3 = new Fl_Window(width, height, "Simple Information");


    Fl_Box *title = new Fl_Box(width/2-200, 40, 400, 80, "Network information");

    title->box(_FL_PLASTIC_ROUND_UP_BOX);
    title->labelfont(FL_BOLD+FL_ITALIC);
    title->labelsize(36);
    title->labeltype(FL_SHADOW_LABEL);

    #include <net/if.h>
    #include <sys/ioctl.h>

    struct ifreq ifr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, "eth0");
    ioctl(sock, SIOCGIFADDR, &ifr);

    Fl_Box *info_main = new Fl_Box(20, 180, width-30, 600);


    info_main->box(_FL_PLASTIC_ROUND_UP_BOX);
    info_main->labelfont(FL_BOLD+FL_ITALIC);
    info_main->labelsize(18);
    info_main->labeltype(FL_SHADOW_LABEL);
    // The text in the text box is place in the for loops and if statements below
    // info_main->copy_label(( ).c_str());
    info_main->align(FL_ALIGN_CENTER);
    
    string temp_string = "";
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        if (ifa->ifa_addr->sa_family == AF_INET) {
            char address[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, address, INET_ADDRSTRLEN);
            temp_string += ifa->ifa_name;
            temp_string += ": ";
            temp_string += address;
            temp_string += "\n";
        } else if (ifa->ifa_addr->sa_family == AF_INET6) {
            char address[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr, address, INET6_ADDRSTRLEN);
            temp_string += ifa->ifa_name;
            temp_string += ": ";
            temp_string += address;
            temp_string += "\n";

        }
    }

    info_main->copy_label(temp_string.c_str());

    freeifaddrs(ifaddr);
    window3->show();

}

// button for network information, calling function network information
void network_info_btn(Fl_Widget* button, void* data){

    network_info();
}


void disk_info() {
    Fl_Window *disk_window = new Fl_Window(860,350,"Disk Information");

    //Create a text output widget
    Fl_Multiline_Output *disk_output = new Fl_Multiline_Output(20, 20, 820, 310);
    disk_output->textsize(18);

    //Execute the command and read the output
    FILE *fp = popen("df -h", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    char output[1024];
    string disk_info;
    while (fgets(output, sizeof(output)-1, fp) != NULL) {
        disk_info += output;
    }

    //set the value of the text output widget
    disk_output->value(disk_info.c_str());

    //add the text output widget to the window
    disk_window->end();
    disk_window->show();

}

void disk_info_btn(Fl_Widget* button, void* data){
    disk_info();
}

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(600,800);
    
    window->color(fl_rgb_color(48, 43, 39));


    int width = 600;
    int height = 800;

    Fl_Box *title = new Fl_Box(width/2-280,50,570,80, "Computer Information Utility");

    title->box(_FL_PLASTIC_ROUND_UP_BOX);
    title->labelfont(FL_BOLD+FL_ITALIC);
    title->labelsize(36);
    title->color(fl_rgb_color(27, 38, 79)); //sets the background color to white
    title->labelcolor(fl_rgb_color(245, 243, 245));
    title->labeltype(FL_SHADOW_LABEL);
    title->box(FL_BORDER_BOX);

    Fl_Button *simple_info = new Fl_Button(100, 200, 400, 60, "Simple information");

    simple_info->box(_FL_PLASTIC_ROUND_UP_BOX);
    simple_info->labelfont(FL_BOLD+FL_ITALIC);
    simple_info->labelsize(36);
    simple_info->color(fl_rgb_color(27, 38, 79)); //sets the background color to white
    simple_info->labelcolor(fl_rgb_color(245, 243, 245));
    simple_info->labeltype(FL_SHADOW_LABEL);
    simple_info->box(FL_BORDER_BOX);
    //call the funciton, and exit the program.
    simple_info->callback(simple_info_btn);


    Fl_Button *network_info = new Fl_Button(100, 270, 400, 60, "Network informaiton");

    network_info->box(_FL_PLASTIC_ROUND_UP_BOX);
    network_info->labelfont(FL_BOLD+FL_ITALIC);
    network_info->labelsize(36);
    network_info->color(fl_rgb_color(27, 38, 79)); //sets the background color to white
    network_info->labelcolor(fl_rgb_color(245, 243, 245));
    network_info->labeltype(FL_SHADOW_LABEL);
    network_info->box(FL_BORDER_BOX);
    //call the funciton, and exit the program.
    network_info->callback(network_info_btn);

    Fl_Button *disk_info = new Fl_Button(100,340, 400, 60, "Disk Information");

    disk_info->box(_FL_PLASTIC_ROUND_UP_BOX);
    disk_info->labelfont(FL_BOLD+FL_ITALIC);
    disk_info->labelsize(36);
    disk_info->color(fl_rgb_color(27, 38, 79)); //sets the background color to white
    disk_info->labelcolor(fl_rgb_color(245, 243, 245));
    disk_info->labeltype(FL_SHADOW_LABEL);
    disk_info->box(FL_BORDER_BOX);
    disk_info->callback(disk_info_btn);

    Fl_Button *ext_btn = new Fl_Button(100, 700, 400, 60, "EXIT");

    ext_btn->box(_FL_PLASTIC_ROUND_UP_BOX);
    ext_btn->labelfont(FL_BOLD+FL_ITALIC);
    ext_btn->labelsize(36);
    ext_btn->color(fl_rgb_color(27, 38, 79)); //sets the background color to white
    ext_btn->labelcolor(fl_rgb_color(245, 243, 245));
    ext_btn->labeltype(FL_SHADOW_LABEL);
    ext_btn->box(FL_BORDER_BOX);
    //call the funciton, and exit the program.
    ext_btn->callback(exit_button_callback);


    window->end();

    window->show(argc, argv);
    return Fl::run();
}

