$(document).ready(resizeYoutubeVideo);

$(window).resize(resizeYoutubeVideo);

function resizeYoutubeVideo() {
    var demoVideo = $("#indexVideo > iframe");
    demoVideo.height((demoVideo.width() / 1.33) - 10);
    console.log("hi");
}