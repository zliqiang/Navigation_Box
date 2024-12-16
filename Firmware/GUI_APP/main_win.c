
#include "main_win.h"
#include "usart.h"
#include "gnss.h"
#include <math.h>
#include "gpios.h"
#include "imu.h"
#include "timer.h"
#include "exfuns.h"

char *list_buff;

static void write_create(lv_obj_t * parent);
static void text_area_event_handler(lv_obj_t * text_area, lv_event_t event);
static void keyboard_event_cb(lv_obj_t * keyboard, lv_event_t event);

static void list_create(lv_obj_t * parent);
static void chart_create(lv_obj_t * parent);
static void slider_event_handler(lv_obj_t * slider, lv_event_t event);
static void list_btn_event_handler(lv_obj_t * slider, lv_event_t event);
static void sd_btn_event_handler(lv_obj_t * obj,lv_event_t event);
void gps_data_update(lv_task_t * task);
void sensor_data_update(lv_task_t * task);
static void list_event_handler(lv_obj_t * obj,lv_event_t event);
void main_table_create();
void sensor_table_create();
void setting_table_create();

static lv_obj_t * chart;
static lv_obj_t * ta;
static lv_obj_t * kb;

static lv_style_t style_kb;
static lv_style_t style_kb_rel;
static lv_style_t style_kb_pr;

static lv_obj_t * latitude_label;
static lv_obj_t * longitude_label;
static lv_obj_t * carrier_label;
static lv_obj_t * svnum_label;


static lv_obj_t * gps_latitude_label;
static lv_obj_t * gps_longitude_label;
static lv_obj_t * gps_carrier_label;
static lv_obj_t * gps_svnum_label;
static lv_obj_t *sd_read_btn,*sd_state_label;

static lv_obj_t * pitch_label;
static lv_obj_t * roll_label;
static lv_obj_t * yaw_label;
static lv_obj_t * ax_label;
static lv_obj_t * ay_label;
static lv_obj_t * az_label;
static lv_obj_t * gx_label;
static lv_obj_t * gy_label;
static lv_obj_t * gz_label;
static lv_obj_t * time_label,*time_label2;
static lv_obj_t * temp_label;

static lv_obj_t * imu_pitch_label;
static lv_obj_t * imu_roll_label;
static lv_obj_t * imu_yaw_label;
static lv_obj_t * imu_ax_label;
static lv_obj_t * imu_ay_label;
static lv_obj_t * imu_az_label;
static lv_obj_t * imu_gx_label;
static lv_obj_t * imu_gy_label;
static lv_obj_t * imu_gz_label;
static lv_obj_t * imu_time_label,*imu_time_label2;
static lv_obj_t * imu_temp_label;

static lv_style_t bg_style;
static lv_style_t sel_style;
static lv_obj_t * station_list;

static lv_obj_t * sd_led, * imu_led, * gnss_led, * time_display,*ble_led;
static lv_obj_t * sd_led_label,* imu_led_label,* gnss_led_label,* time_display_label,*ble_led_label;

lv_style_t bg_style;
lv_style_t indic_style;
lv_style_t btn_bg_style;
lv_style_t btn_rel_style;
lv_style_t btn_pr_style;
lv_style_t btn_tgl_rel_style;
lv_style_t btn_tgl_pr_style;
lv_obj_t * tabview1;
bool is_sliding = true;
bool is_btns_hidden = false;

u8 sd_state = 0;
u8 ble_init_success=0;


void main_win_create(void)
{
    lv_obj_t *scr = lv_scr_act();//获取当前活跃的屏幕对象

  //1.创建样式
  //1.1 创建背景样式
  lv_style_copy(&bg_style,&lv_style_plain);
  bg_style.body.main_color = LV_COLOR_MAKE(49,49,49);//纯色背景
  bg_style.body.grad_color = bg_style.body.main_color;
  bg_style.body.border.color = LV_COLOR_MAKE(150,150,150);//边框颜色
  bg_style.body.border.width = 2;//边框宽度
  bg_style.text.color = LV_COLOR_WHITE;
  //1.2 创建页面指示器的样式
  lv_style_copy(&indic_style,&lv_style_plain_color);
  indic_style.body.main_color = LV_COLOR_MAKE(42,212,66);//指示器的颜色,绿色
  indic_style.body.grad_color = indic_style.body.main_color;
  indic_style.body.padding.inner = 3;//设置指示器的高度
  //1.3 创建页面选择按钮栏的背景样式
  //lv_style_transp_tight样式中的inner,left,top,right,bottom等内间距值都为0,这是为了让页面选择按钮能够紧挨在一起
  lv_style_copy(&btn_bg_style,&lv_style_transp_tight);
  //1.4 创建按钮正常态下的松手样式
  lv_style_copy(&btn_rel_style,&lv_style_plain_color);
  btn_rel_style.body.main_color = LV_COLOR_MAKE(98,98,98);
  btn_rel_style.body.grad_color = btn_rel_style.body.main_color;
  btn_rel_style.body.border.color = LV_COLOR_MAKE(150,150,150);//边框颜色
  btn_rel_style.body.border.width = 1;
  btn_rel_style.text.color = LV_COLOR_WHITE;//字体颜色
  //1.5 创建按钮正常态下的按下样式
  lv_style_copy(&btn_pr_style,&btn_rel_style);
  btn_pr_style.body.main_color = LV_COLOR_GRAY;
  btn_pr_style.body.grad_color = btn_pr_style.body.main_color;
  //1.6 创建按钮切换态下的松手样式
  lv_style_copy(&btn_tgl_rel_style,&btn_rel_style);
  btn_tgl_rel_style.body.main_color = bg_style.body.main_color;//和主背景颜色一致
  btn_tgl_rel_style.body.grad_color = btn_tgl_rel_style.body.main_color;
  //1.7 创建按钮切换态下的按下样式
  //保持和btn_tgl_rel_style一样就行了
  lv_style_copy(&btn_tgl_pr_style,&btn_tgl_rel_style);

  //2.创建选项卡
  tabview1 = lv_tabview_create(scr,NULL);
  lv_obj_set_size(tabview1,lv_obj_get_width(scr),lv_obj_get_height(scr));//设置选项卡的大小,比屏幕小16像素
  lv_obj_align(tabview1,NULL,LV_ALIGN_CENTER,0,0);//与屏幕居中对齐
  
  lv_tabview_set_btns_pos(tabview1,LV_TABVIEW_BTNS_POS_TOP);//设置页面选择按钮栏位于顶部
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_BG,&bg_style);//设置背景样式
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_INDIC,&indic_style);//设置页面指示器的样式
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_BTN_BG,&btn_bg_style);//设置页面选择按钮栏的背景样式
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_BTN_REL,&btn_rel_style);//设置按钮正常态下的松手样式
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_BTN_PR,&btn_pr_style);//设置按钮正常态下的按下样式
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_BTN_TGL_REL,&btn_tgl_rel_style);//设置按钮切换态下的松手样式
  lv_tabview_set_style(tabview1,LV_TABVIEW_STYLE_BTN_TGL_PR,&btn_tgl_pr_style);//设置按钮切换态下的按下样式
  //2.1 添加tab1内容页面
    main_table_create();
  //2.2 添加tab2内容页面
	sensor_table_create();

  //2.3 添加tab3内容页面
	setting_table_create();
    
}


void main_table_create()
{
	lv_obj_t *tab1_page = lv_tabview_add_tab(tabview1,LV_SYMBOL_WIFI" NAV");
     latitude_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(latitude_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(latitude_label,130);
    lv_label_set_align(latitude_label,LV_LABEL_ALIGN_LEFT);
    lv_label_set_static_text(latitude_label,"latitude:");
    lv_obj_set_pos(latitude_label,0,60);
	//lv_obj_align(pitch_label,NULL,LV_ALIGN_CENTER,0,0);//设置对齐方式

    gps_latitude_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(gps_latitude_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(gps_latitude_label,100);
    lv_label_set_text(gps_latitude_label,"0");
    lv_obj_set_pos(gps_latitude_label,130,60);
    //lv_obj_align(imu_pitch_label,pitch_label,LV_ALIGN_OUT_TOP_RIGHT,0,0);//设置对齐方式
    lv_label_set_body_draw(gps_latitude_label,true);//使能背景重绘
    lv_label_set_recolor(gps_latitude_label,true);//使能文本重绘色

    longitude_label = lv_label_create(tab1_page,latitude_label);
    lv_label_set_static_text(longitude_label,"longitude:");
    lv_obj_set_pos(longitude_label,0,90);

    gps_longitude_label = lv_label_create(tab1_page,gps_latitude_label);
    lv_obj_set_pos(gps_longitude_label,130,90);

    svnum_label = lv_label_create(tab1_page,latitude_label);
    lv_label_set_static_text(svnum_label,"longitude:");
    lv_obj_set_pos(svnum_label,0,120);

    gps_svnum_label = lv_label_create(tab1_page,gps_latitude_label);
    lv_obj_set_pos(gps_svnum_label,130,120);

    static lv_style_t sd_led_style;
    lv_style_copy(&sd_led_style, &lv_style_plain_color);//样式拷贝
    sd_led_style.body.radius = LV_RADIUS_CIRCLE;//绘制半圆角
	if(sd_state == 0)
 	{   //主背景的上半部分的颜色
    	sd_led_style.body.main_color = LV_COLOR_MAKE(0, 0XFF, 0);
    	//主背景的下半部分的颜色
    	sd_led_style.body.grad_color = LV_COLOR_MAKE(0, 0XFF, 0);
	}
	else 
	{
		//主背景的上半部分的颜色
    	sd_led_style.body.main_color = LV_COLOR_MAKE(0xff, 0, 0);
    	//主背景的下半部分的颜色
    	sd_led_style.body.grad_color = LV_COLOR_MAKE(0xff, 0, 0);
	}
    sd_led_style.body.border.opa = LV_OPA_COVER;//透明度
    sd_led_style.body.border.color = LV_COLOR_MAKE(0xff, 0xff, 0xff);//边框颜色
    sd_led_style.body.border.width = 1;//边框的宽度

    sd_led = lv_led_create(tab1_page, NULL);
    lv_obj_set_pos(sd_led,80,200);//设置坐标
    lv_obj_set_size(sd_led,20,20);//设置大小
    lv_obj_set_style(sd_led, &sd_led_style);//设置样式
    lv_led_set_bright(sd_led,255);

    
    static lv_style_t ble_led_style;
    lv_style_copy(&ble_led_style, &lv_style_plain_color);//样式拷贝
    ble_led_style.body.radius = LV_RADIUS_CIRCLE;//绘制半圆角
	if(ble_init_success == 1)
 	{   //主背景的上半部分的颜色
    	ble_led_style.body.main_color = LV_COLOR_MAKE(0, 0XFF, 0);
    	//主背景的下半部分的颜色
    	ble_led_style.body.grad_color = LV_COLOR_MAKE(0, 0XFF, 0);
	}
	else 
	{
		//主背景的上半部分的颜色
    	ble_led_style.body.main_color = LV_COLOR_MAKE(0xff, 0, 0);
    	//主背景的下半部分的颜色
    	ble_led_style.body.grad_color = LV_COLOR_MAKE(0xff, 0, 0);
	}
    ble_led_style.body.border.opa = LV_OPA_COVER;//透明度
    ble_led_style.body.border.color = LV_COLOR_MAKE(0xff, 0xff, 0xff);//边框颜色
    ble_led_style.body.border.width = 1;//边框的宽度

    ble_led = lv_led_create(tab1_page, NULL);
    lv_obj_set_pos(ble_led,380,200);//设置坐标
    lv_obj_set_size(ble_led,20,20);//设置大小
    lv_obj_set_style(ble_led, &ble_led_style);//设置样式
    lv_led_set_bright(ble_led,255);

	 static lv_style_t imu_led_style;
    lv_style_copy(&imu_led_style, &lv_style_plain_color);//样式拷贝
    imu_led_style.body.radius = LV_RADIUS_CIRCLE;//绘制半圆角
    //主背景的上半部分的颜色
    imu_led_style.body.main_color = LV_COLOR_MAKE(0, 0XFF, 0);

    //主背景的下半部分的颜色
    imu_led_style.body.grad_color = LV_COLOR_MAKE(0, 0XFF, 0);
    imu_led_style.body.border.opa = LV_OPA_COVER;//透明度
    imu_led_style.body.border.color = LV_COLOR_MAKE(0xff, 0xff, 0xff);//边框颜色
    imu_led_style.body.border.width = 1;//边框的宽度

    imu_led = lv_led_create(tab1_page, NULL);
    lv_obj_set_pos(imu_led,180,200);//设置坐标
    lv_obj_set_size(imu_led,20,20);//设置大小
    lv_obj_set_style(imu_led, &imu_led_style);//设置样式
    lv_led_set_bright(imu_led,255);

	 static lv_style_t gnss_led_style;
    lv_style_copy(&gnss_led_style, &lv_style_plain_color);//样式拷贝
    gnss_led_style.body.radius = LV_RADIUS_CIRCLE;//绘制半圆角
    //主背景的上半部分的颜色
    gnss_led_style.body.main_color = LV_COLOR_MAKE(0, 0XFF, 0);

    //主背景的下半部分的颜色
    gnss_led_style.body.grad_color = LV_COLOR_MAKE(0, 0XFF, 0);
    gnss_led_style.body.border.opa = LV_OPA_COVER;//透明度
    gnss_led_style.body.border.color = LV_COLOR_MAKE(0xff, 0xff, 0xff);//边框颜色
    gnss_led_style.body.border.width = 1;//边框的宽度

    gnss_led = lv_led_create(tab1_page, NULL);
    lv_obj_set_pos(gnss_led,280,200);//设置坐标
    lv_obj_set_size(gnss_led,20,20);//设置大小
    lv_obj_set_style(gnss_led, &gnss_led_style);//设置样式
    lv_led_set_bright(gnss_led,255);

    lv_led_on(sd_led);
    lv_led_on(imu_led);
    lv_led_on(gnss_led);
    lv_led_on(ble_led);

    

	// static lv_style_t label_style;
	// lv_style_copy(&label_style,&lv_style_plain_color);
	// label_style.text.font = &lv_font_roboto_16;
    ble_led_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(ble_led_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(ble_led_label,130);
    lv_label_set_align(ble_led_label,LV_LABEL_ALIGN_LEFT);
	//lv_label_set_style(sd_led_label,LV_LABEL_STYLE_MAIN,&label_style);
    lv_label_set_static_text(ble_led_label,"BLE");
    lv_obj_set_pos(ble_led_label,320,200);

    sd_led_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(sd_led_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(sd_led_label,130);
    lv_label_set_align(sd_led_label,LV_LABEL_ALIGN_LEFT);
	//lv_label_set_style(sd_led_label,LV_LABEL_STYLE_MAIN,&label_style);
    lv_label_set_static_text(sd_led_label,"SDCard");
    lv_obj_set_pos(sd_led_label,20,200);

    imu_led_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(imu_led_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(imu_led_label,130);
    lv_label_set_align(imu_led_label,LV_LABEL_ALIGN_LEFT);
    lv_label_set_static_text(imu_led_label,"IMU");
    lv_obj_set_pos(imu_led_label,120,200);

    gnss_led_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(gnss_led_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(gnss_led_label,130);
    lv_label_set_align(gnss_led_label,LV_LABEL_ALIGN_LEFT);
    lv_label_set_static_text(gnss_led_label,"GNSS");
    lv_obj_set_pos(gnss_led_label,220,200);

	time_display_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(time_display_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(time_display_label,130);
    lv_label_set_align(time_display_label,LV_LABEL_ALIGN_LEFT);
    lv_label_set_static_text(time_display_label,"Time:");
    lv_obj_set_pos(time_display_label,300,60);
	//lv_obj_align(time_display_label,NULL,LV_ALIGN_CENTER,0,0);//设置对齐方式

    time_display = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(time_display,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(time_display,100);
    lv_label_set_text(time_display,"0");
    lv_obj_set_pos(time_display,380,60);
    //lv_obj_align(time_display,pitch_label,LV_ALIGN_OUT_TOP_RIGHT,0,0);//设置对齐方式
    lv_label_set_body_draw(time_display,true);//使能背景重绘
    lv_label_set_recolor(time_display,true);//使能文本重绘色



    lv_task_create(gps_data_update,500,LV_TASK_PRIO_MID,NULL);

    sd_read_btn = lv_btn_create(tab1_page,NULL);
    lv_obj_set_width(sd_read_btn,100);
    lv_obj_set_pos(sd_read_btn,350,90);
    lv_obj_set_event_cb(sd_read_btn,sd_btn_event_handler);//设置事件回调函数
    
    sd_state_label = lv_label_create(tab1_page,NULL);
    lv_label_set_long_mode(sd_state_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(sd_state_label,130);
    lv_label_set_align(sd_state_label,LV_LABEL_ALIGN_LEFT);
    lv_label_set_static_text(sd_state_label,"SD_STATE:");
    lv_obj_set_pos(sd_state_label,350,150);

}

void sensor_table_create()
{
	 lv_obj_t *tab2_page = lv_tabview_add_tab(tabview1,LV_SYMBOL_AUDIO" sensor");
    //往内容页面2中添加标签子对象
    
    pitch_label = lv_label_create(tab2_page,NULL);
    lv_label_set_long_mode(pitch_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(pitch_label,130);
    lv_label_set_align(pitch_label,LV_LABEL_ALIGN_LEFT);
    lv_label_set_static_text(pitch_label,"Pitch:");
    lv_obj_set_pos(pitch_label,0,0);
	//lv_obj_align(pitch_label,NULL,LV_ALIGN_CENTER,0,0);//设置对齐方式

    imu_pitch_label = lv_label_create(tab2_page,NULL);
    lv_label_set_long_mode(imu_pitch_label,LV_LABEL_LONG_BREAK);
    lv_obj_set_width(imu_pitch_label,100);
    lv_label_set_text(imu_pitch_label,"0");
    lv_obj_set_pos(imu_pitch_label,50,0);
    //lv_obj_align(imu_pitch_label,pitch_label,LV_ALIGN_OUT_TOP_RIGHT,0,0);//设置对齐方式
    lv_label_set_body_draw(imu_pitch_label,true);//使能背景重绘
    lv_label_set_recolor(imu_pitch_label,true);//使能文本重绘色

    roll_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(roll_label,"Roll:");
    lv_obj_set_pos(roll_label,1,30);

    imu_roll_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_roll_label,51,30);

    yaw_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(yaw_label,"Yaw:");
    lv_obj_set_pos(yaw_label,2,60);

    imu_yaw_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_yaw_label,52,60);

    ax_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(ax_label,"ax:");
    lv_obj_set_pos(ax_label,3,90);

    imu_ax_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_ax_label,53,90);

    ay_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(ay_label,"ay:");
    lv_obj_set_pos(ay_label,4,120);

    imu_ay_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_ay_label,54,120);

    az_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(az_label,"az:");
    lv_obj_set_pos(az_label,5,150);

    imu_az_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_az_label,55,150);
		
	gx_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(gx_label,"gz:");
    lv_obj_set_pos(gx_label,200,0);

    imu_gx_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_gx_label,250,0);
		
	gy_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(gy_label,"gy:");
    lv_obj_set_pos(gy_label,201,30);

    imu_gy_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_gy_label,251,30);
		
	gz_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(gz_label,"gz:");
    lv_obj_set_pos(gz_label,202,60);

    imu_gz_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_gz_label,252,60);
    
    temp_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(temp_label,"temp:");
    lv_obj_set_pos(temp_label,6,180);

    imu_temp_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_temp_label,56,180);

    time_label = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(time_label,"time:");
    lv_obj_set_pos(time_label,7,210);

    imu_time_label = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_time_label,57,210);

    time_label2 = lv_label_create(tab2_page,pitch_label);
    lv_label_set_static_text(time_label2,"time:");
    lv_obj_set_pos(time_label2,203,90);

    imu_time_label2 = lv_label_create(tab2_page,imu_pitch_label);
    lv_obj_set_pos(imu_time_label2,203,90);

    lv_task_create(sensor_data_update,500,LV_TASK_PRIO_MID,NULL);


}

void setting_table_create()
{
	 lv_obj_t *tab3_page = lv_tabview_add_tab(tabview1,LV_SYMBOL_BELL" Setting");
  //往内容页面3中添加标签子对象
   
	lv_style_copy(&bg_style,&lv_style_plain);
	bg_style.body.main_color = LV_COLOR_WHITE;
	bg_style.body.grad_color = bg_style.body.main_color;
	bg_style.body.border.width = 1;
	bg_style.body.border.color = LV_COLOR_MAKE(0x5F,0xB8,0x78);
	sel_style.body.grad_color = sel_style.body.main_color;
	sel_style.text.color = LV_COLOR_WHITE;//文本为白色

	
	station_list = lv_ddlist_create(tab3_page,NULL);
	lv_ddlist_set_options(station_list,list_buff);//设置列表选项
	lv_ddlist_set_selected(station_list,0);//设置默认选中值为 Hangzhou
	lv_ddlist_set_fix_width(station_list,140);//设置固定宽度
	lv_ddlist_set_draw_arrow(station_list,true);//使能绘制向下的箭头
	lv_ddlist_set_style(station_list,LV_DDLIST_STYLE_BG,&bg_style);//设置背景样式
	lv_ddlist_set_style(station_list,LV_DDLIST_STYLE_SEL,&sel_style);//设置背景样式
	lv_obj_set_event_cb(station_list, list_event_handler);//注册事件回调函数
	lv_obj_align(station_list,NULL,LV_ALIGN_IN_TOP_MID,0,20);//设置与屏幕的对齐方式
}

static void list_event_handler(lv_obj_t * obj,lv_event_t event)
{
	if(event == LV_EVENT_VALUE_CHANGED)
	{
		char buf[32];
		uint16_t selected_index = lv_ddlist_get_selected(obj);//获取选项值的索引
		lv_ddlist_get_selected_str(obj,buf,sizeof(buf));//获取选项值的文本内容
		printf("Option index: %d,Option text:%s\r\n",selected_index,buf);
	}
}


void gps_data_update(lv_task_t * task)
{
    char buff[15];
	
	
    // imu_pitch =imu_data.data[0];
    // imu_roll  = imu_data.data[1];
    

    // imu_pitch =imu_data_global[0];
    // imu_roll  = imu_data_global[1];
    // imu_yaw   = imu_data_global[2];
    // imu_ax    = imu_data_global[3];
    // imu_ay    = imu_data_global[4];
    // imu_az    = imu_data_global[5]; 
    // imu_temp_dp = imu_temp_global;
    // imu_time_dp = imu_time_global;
	  
    sprintf(buff,"%f",latitude);
	//sprintf(buff,"#%s %f#",imu_pitch<60?"00FF00":(imu_pitch<90?"FFFF00":"FF0000"),imu_pitch); 
    lv_label_set_text(gps_latitude_label,buff);
    sprintf(buff,"%f",longitude); 
    lv_label_set_text(gps_longitude_label,buff);
    sprintf(buff,"%d",gnss_pvt_numSV.numSV);
	//sprintf(buff,"#%s %f#",imu_pitch<60?"00FF00":(imu_pitch<90?"FFFF00":"FF0000"),imu_pitch); 
    lv_label_set_text(gps_svnum_label,buff);


	sprintf(buff,"%f",time_write);
	lv_label_set_text(time_display,buff);

}

//任务回调函数
void sensor_data_update(lv_task_t * task)
{

    char buff[40];
   
	float imu_pitch0 = 0,imu_roll0 = 0,imu_yaw0 = 0,imu_ax0 = 0,imu_ay0 = 0,imu_az0 = 0,imu_temp_dp = 0,imu_gx0 = 0,imu_gy0 =0,imu_gz0 =0,imu_time_dp,imu_time_dp2;
	
    // imu_pitch =imu_data.data[0];
    // imu_roll  = imu_data.data[1];
    imu_pitch0 = imu_pitch.pitch;
    imu_roll0 = imu_roll.roll;

    imu_yaw0   = imu_yaw.yaw;
    imu_ax0    = imu_ax.ax;
    imu_ay0    = imu_ay.ay;
    imu_az0    = imu_az.az; 
    imu_gx0    = imu_gx.gx;
    imu_gy0    = imu_gy.gy;
    imu_gz0    = imu_gz.gz; 
    imu_temp_dp = imu_temp.temp;
    imu_time_dp = imu_time_s;
    imu_time_dp2 = time_write;

    // imu_pitch =imu_data_global[0];
    // imu_roll  = imu_data_global[1];
    // imu_yaw   = imu_data_global[2];
    // imu_ax    = imu_data_global[3];
    // imu_ay    = imu_data_global[4];
    // imu_az    = imu_data_global[5]; 
    // imu_temp_dp = imu_temp_global;
    // imu_time_dp = imu_time_global;
	  
    
	sprintf(buff,"%f",imu_pitch0); 
    lv_label_set_text(imu_pitch_label,buff);
    sprintf(buff,"%f",imu_roll0); 
    lv_label_set_text(imu_roll_label,buff);
    sprintf(buff,"%f",imu_yaw0); 
    lv_label_set_text(imu_yaw_label,buff);
    sprintf(buff,"%f",imu_ax0); 
    lv_label_set_text(imu_ax_label,buff);
    sprintf(buff,"%f",imu_ay0); 
    lv_label_set_text(imu_ay_label,buff);
    sprintf(buff,"%f",imu_az0); 
	lv_label_set_text(imu_az_label,buff);
    sprintf(buff,"%f",imu_gx0); 
    lv_label_set_text(imu_gx_label,buff);
    sprintf(buff,"%f",imu_gy0); 
    lv_label_set_text(imu_gy_label,buff);
    sprintf(buff,"%f",imu_gz0); 
	lv_label_set_text(imu_gz_label,buff);
    sprintf(buff,"%f",imu_temp_dp); 
	lv_label_set_text(imu_temp_label,buff);
    sprintf(buff,"%f",imu_time_dp); 
	lv_label_set_text(imu_time_label,buff);
    sprintf(buff,"%f",imu_time_dp2); 
	lv_label_set_text(imu_time_label2,buff);

}

static void sd_btn_event_handler(lv_obj_t * obj,lv_event_t event)
{
    if(event==LV_EVENT_PRESSED)
    {
        
        lv_label_set_static_text(sd_state_label,"writing");
        f_close(file);
        lv_label_set_static_text(sd_state_label,"over");
    }
}

