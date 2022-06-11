

#include "main.h"
#include "web_serve.h"
#include "EPD.h"

uint8_t gImage111[2756]={0};
static SemaphoreHandle_t Web_Epd_Thresholding_Date_Semap;

struct file_server_data
{
    /* Base path of file storage */
    char base_path[ESP_VFS_PATH_MAX + 1];

    /* Scratch buffer for temporary storage during file transfer */
    char scratch[SCRATCH_BUFSIZE];
};

typedef struct rest_server_context
{
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)
/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html"))
    {
        type = "text/html";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".js"))
    {
        type = "application/javascript";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".css"))
    {
        type = "text/css";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".png"))
    {
        type = "image/png";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".ico"))
    {
        type = "image/x-icon";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".svg"))
    {
        type = "text/xml";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".jpg"))
    {
        type = "image/jpg";
    }
    return httpd_resp_set_type(req, type);
}

static esp_err_t ret_upload_post_handler(httpd_req_t *req)
{
    char *buf = ((struct file_server_data *)req->user_ctx)->scratch;
    int remaining = req->content_len;

    int received;
    char *filepath = "/www/sa.txt";
    ESP_LOGI(TAG, "filepath : %s", filepath);
    FILE *fd = NULL;
    fd = fopen(filepath, "w");
    if (!fd)
    {
        ESP_LOGE(TAG, "Failed to create file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to create file");
        return ESP_FAIL;
    }

    while (remaining > 0)
    {
        ESP_LOGI(TAG, "Remaining size : %d", remaining);
        /* Receive the file part by part into a buffer */
        if ((received = httpd_req_recv(req, buf, MIN(remaining, SCRATCH_BUFSIZE))) <= 0)
        {
            if (received == HTTPD_SOCK_ERR_TIMEOUT)
            {
                /* Retry if timeout occurred */
                continue;
            }
            /* In case of unrecoverable error,
             * close and delete the unfinished file*/
            fclose(fd);
            unlink(filepath);

            ESP_LOGE(TAG, "File reception failed!");
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive file");
            return ESP_FAIL;
        }
        ESP_LOGI(TAG, "received = %d",received);
        /* Write buffer content to file on storage */

        if (received && (received != fwrite(buf, 1, received, fd)))
        {
            /* Couldn't write everything to file!
             * Storage may be full? */
            fclose(fd);
            unlink(filepath);
            ESP_LOGE(TAG, "File write failed!");
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to write file to storage");
            return ESP_FAIL;
        }
            //ESP_LOGI(TAG, "date sum = 2756");
        ESP_LOGI(TAG, "File written");
        /* Keep track of remaining size of
         * the file left to be uploaded */
        remaining -= received;
        }

        fclose(fd);
        ESP_LOGI(TAG, "File reception complete");
        httpd_resp_set_status(req, "303 See Other");
        httpd_resp_set_hdr(req, "Location", "/");
        httpd_resp_sendstr(req, "File uploaded successfully");

        xSemaphoreGive(Web_Epd_Thresholding_Date_Semap);

        return ESP_OK;
}
static esp_err_t rest_common_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
    strlcpy(filepath, rest_context->base_path, sizeof(filepath));

    ESP_LOGI(TAG, "filepath=%s req->uri=%s \n", filepath, req->uri);
    if (req->uri[strlen(req->uri) - 1] == '/')
    {
        strlcat(filepath, "/index.html", sizeof(filepath));
    }
    else
    {
        strlcat(filepath, req->uri, sizeof(filepath));
    }

    char file_path[FILE_PATH_MAX] = "/www";
    strlcat(file_path, filepath, sizeof(filepath));
    strlcpy(filepath, file_path, sizeof(filepath));
    ESP_LOGI(TAG, "filepath=%s \n", filepath);

    ESP_LOGI(TAG, "Opening file");
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1)
    {
        ESP_LOGE(TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);

    char *chunk = malloc(sizeof(char) * SCRATCH_BUFSIZE);
    ssize_t read_bytes;
    do
    {
        /* Read file in chunks into the scratch buffer */
        // read_bytes = read(fd, chunk, 200);
        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1)
        {
            ESP_LOGE(TAG, "Failed to read file : %s", filepath);
        }
        else if (read_bytes > 0)
        {
            httpd_resp_send_chunk(req, chunk, read_bytes);
        }
    } while (read_bytes > 0);

    close(fd);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}


void Web_Epd_Thresholding_Date_Task(void *pvParam)
{
    while (1)
    {
        if(xSemaphoreTake(Web_Epd_Thresholding_Date_Semap, 10) == pdTRUE)
        {
            printf("Web_Epd_Thresholding_Date_Task\n");
            FILE *fd = NULL;
            char *filepath = "/www/sa.txt";
            fd = fopen(filepath, "w");
            if (!fd)
            {
                ESP_LOGE(TAG, "Failed to create file : %s \n", filepath);
            }

            if(fread(gImage111,1,2756,fd))
            {
                ESP_LOGE(TAG, "Failed Open file \n");
            }
            PIC_display_Clean();
            printf("完成刷新");
        }
        vTaskDelay( 1 / portTICK_PERIOD_MS);
    }

}

void html_int()
{
    rest_server_context_t *rest_context = calloc(1, sizeof(rest_server_context_t));
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    Web_Epd_Thresholding_Date_Semap=xSemaphoreCreateBinary();
    xTaskCreate(Web_Epd_Thresholding_Date_Task,"Web_Epd",2048,NULL,1,NULL);

    ESP_LOGI(TAG, "Starting HTTP Server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start file server!");
    }

    /* URI handler for getting web server files */
    httpd_uri_t common_get_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = rest_common_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &common_get_uri);

    httpd_uri_t file_upload = {
        .uri = "/upload/*", // Match all URIs of type /upload/path/to/file
        .method = HTTP_POST,
        .handler = ret_upload_post_handler,
        .user_ctx = rest_context // Pass server data as context
    };
    httpd_register_uri_handler(server, &file_upload);
}
