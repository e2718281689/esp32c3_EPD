let imgElement = document.getElementById('imageUpload');
let inputElement = document.getElementById('fileInput');

inputElement.onchange = function() {
    imgElement.src = URL.createObjectURL(event.target.files[0]);
}

function test()
{
    let  xxx;
    xxx=document.getElementById("data").value;
    console.log(xxx);
    let upload_path = "/upload/" + xxx;
    let xhttp = new XMLHttpRequest();
    xhttp.open("POST", upload_path, true);
    xhttp.send(xxx);

}
function image_update()
{

    // let buffer = new ArrayBuffer(212*104/8);
    // let image = new Uint8Array(buffer)
    // //var image=new Array();
    // let src = cv.imread('imageUpload');
    // let dst = new cv.Mat();
    // let dsize = new cv.Size(212, 104);
    // cv.resize(src, src, dsize, 0, 0, cv.INTER_AREA);
    // cv.cvtColor(src, src, cv.COLOR_RGBA2GRAY, 0);
    // cv.adaptiveThreshold(src, dst, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 3, 2);
    // cv.imshow('canvasOutput', dst);
    // for (let col = 0; col < 212; col++) {
    //     for (let row = 0; row <104; row++) {
    //         let h=dst.data[row * dst.cols * dst.channels() + col * dst.channels()];
    //         //console.log("img="+image[212*row+col]+"row="+row+"col="+col);
    //         //黑为0 白为1
    //         if(Number(h)==0)
    //         {
    //             //image[Math.trunc(row/8)+col*13] 纵走
    //             //image[Math.trunc(row/8)+Math.trunc(row/8)*212] 横走
    //             image[image[Math.trunc(row/8)+col*13]]&= ~(1 << (row%8));
    //         }
    //         else if(Number(h)==255)
    //         {
    //             image[Math.trunc(row/8)+col*13]|= 0x01 << (row % 8);
    //         }
    //         //console.log("img="+image[Math.trunc(row/8)+col*13]+"row="+row+"col="+col);
    //     }
    // }
    // let ooo={
    //     "name":"image",
    //     "date":""
    // };
    //
    // // for (let index = 0; index < 212*104/8; index++) {
    // //     ooo.date=ooo.date+String.fromCharCode(image[index]);
    // // }
    // for (let col = 0; col < 212; col++) {
    //     for (let row = 0; row <13; row++) {
    //         ooo.date=ooo.date+String.fromCharCode(image[row*212+col]);
    //     }
    // }
    // let xhttp = new XMLHttpRequest();
    // xhttp.open("POST", "/upload/image", true);
    // //xhttp.send(JSON.stringify(ooo));
    // //xhttp.send(image);
    // src.delete(); dst.delete();

// <-  -- 212 --  ->
// -               -
// 104           104
// -               -
// <-  -- 212 --  ->


    var myarr=new Array(); //先声明一维
    for(var i=0;i<212;i++){ //一维长度为2
        myarr[i]=new Array(); //再声明二维
        for(var j=0;j<104;j++){ //二维长度为3
            myarr[i][j]=0; // 赋值，每个数组元素的值为i+j
        }
    }

    let src = cv.imread('imageUpload');
    let dst = new cv.Mat();
    let dsize = new cv.Size(212, 104);
    cv.resize(src, src, dsize, 0, 0, cv.INTER_AREA);
    cv.cvtColor(src, src, cv.COLOR_RGBA2GRAY, 0);
    cv.adaptiveThreshold(src, dst, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 3, 2);
    cv.imshow('canvasOutput', dst);

    for (let col = 0; col < 212; col++) {
        for (let row = 0; row <104; row++) {
            let h=dst.data[row * dst.cols * dst.channels() + col * dst.channels()];
            if(Number(h)==0)
            {
                myarr[col][row]=0;
            }
            else if(Number(h)==255)
            {
                myarr[col][row]=255;
            }
        }
    }

    var img_date=new Array(); //先声明一维
    for(var i=0;i<212;i++){ //一维
        let buffer = new ArrayBuffer(104/8);
        img_date[i]=new Uint8Array(buffer); //再声明二维
        for(var j=0;j<104/8;j++){ //二维
            img_date[i][j]=0; //
        }
    }

    for (let col = 0; col < 212; col++) {
        for (let row = 0; row <104/8; row++) {
            for(let date_bit=0;date_bit<8;date_bit++)
            {
                let  xczxcz=myarr[col][row*8+date_bit];
                if(myarr[col][row*8+date_bit]==255)
                {
                    img_date[col][row]|=(1<<date_bit);
                }
                if(myarr[col][row*8+date_bit]==0)
                {
                    img_date[col][row]&=(~(1<<date_bit));
                }

            }
        }
    }

    let buffer_xxx = new ArrayBuffer(212*104/8);
    img_date_xxx=new Uint8Array(buffer_xxx);



    console.log(img_date);

    let img_date_sum=0;
    for (let col = 0; col < 212; col++) {
        for (let row = 0; row <13; row++) {
            let fdsfsdf=img_date[col][row];
            //dsdds=(((fdsfsdf * 0x80200802) & 0x0884422110) * 0x0101010101 >> 32);
            img_date_xxx[img_date_sum]=fdsfsdf;
            img_date_sum++;
        }
    }
    console.log(img_date_xxx);
    let xhttp = new XMLHttpRequest();
    xhttp.open("POST", "/upload/image", true);
    xhttp.send(img_date_xxx);
}
function onOpenCvReady()
{
    console.log("opencv.js加载完成");
}