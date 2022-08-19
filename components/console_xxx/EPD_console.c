//
// Created by cjia on 22-6-25.
//
//
// Created by cjia on 22-6-25.
//
#include "EPD_console.h"
#include <dirent.h>
#include "argtable3/argtable3.h"

static const char *TAG = "EPD_console";


static struct {
    struct arg_int *port;
    struct arg_lit *all;
    struct arg_lit *list;
    struct arg_lit  *ls;
    struct arg_end *end;
} file_ls_args;

static int file_ls_cmd(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **)&file_ls_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, file_ls_args.end, argv[0]);
        return 0;
    }
    if(file_ls_args.all->count>0)
    {
        printf("file_ls_args.all\n");
    }
    if(file_ls_args.list->count>0)
    {
        printf("file_ls_args.list\n");
    }
    if(file_ls_args.port->count>0)
    {
        int chip_addr = file_ls_args.port->ival[0];
        printf("%d \n",chip_addr);
    }
    if(file_ls_args.ls->count>0)
    {
        printf("file_ls_args.ls\n");
    }
    return 0;
}
void file_cmd_init()
{
    file_ls_args.port= arg_int0("c", "chip", "<chip_addr>", "Specify the address of the chip on that bus");
    file_ls_args.all = arg_litn("a","all",0,1,"Show All");
    file_ls_args.list= arg_litn("l","list",0,2,"Show list");
    file_ls_args.ls= arg_litn(NULL,NULL,0,1,"Show ls");

    file_ls_args.end = arg_end(2);
    const esp_console_cmd_t file_cd_cmd = {
            .command = "ls",
            .help = "file show cmd",
            .hint = NULL,
            .func = &file_ls_cmd,
            .argtable = &file_ls_args
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&file_cd_cmd));
}
void register_filetools()
{
    file_cmd_init();
}
void EPD_console_init()
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = "EPD_sss>";
    register_filetools();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}
