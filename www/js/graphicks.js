function repaint_graph(graphData){
    // Lines Graph #############################################
    $.plot($('#graph-lines'), graphData, {
        series: {
            points: {
                show: true,
                radius: 5
            },
            lines: {
                show: true
            },
            shadowSize: 0
        },
        xaxis: {
            mode: "time",
            timeformat: "%y/%m/%d %H:%M:%S",
            autoscaleMargin: 0.02
        }
        ,
        grid: {
            color: '#646464',
            borderColor: 'transparent',
            borderWidth: 20,
            hoverable: true
        }
    }
   
    );

    // Bars Graph ##############################################
    $.plot($('#graph-bars'), graphData, {
        series: {
            bars: {
                show: true,
                barWidth: .9,
                align: 'center'
            },
            shadowSize: 0
        },
        grid: {
            color: '#646464',
            borderColor: 'transparent',
            borderWidth: 20,
            hoverable: true
        }
        ,
        xaxis: {
            mode: "time",
            timeformat: "%y/%m/%d %H:%M:%S",
            autoscaleMargin: 0.02
        },
        yaxis: {
            autoscale:true,
            //tickRenderer: $.jqplot.CanvasAxisTickRenderer,
            tickOptions: {
                labelPosition: 'start'
            }
        }
        ,
        legend: {
            show: true,
            location: 'e'
        }
        
    });
    
    // Graph Toggle ############################################
    $('#graph-bars').hide();

    $('#lines').on('click', function (e) {
        $('#bars').removeClass('active');
        $('#graph-bars').fadeOut();
        $(this).addClass('active');
        $('#graph-lines').fadeIn();
        e.preventDefault();
    });

    $('#bars').on('click', function (e) {
        $('#lines').removeClass('active');
        $('#graph-lines').fadeOut();
        $(this).addClass('active');
        $('#graph-bars').fadeIn().removeClass('hidden');
        e.preventDefault();
    });

    // Tooltip #################################################
    function showTooltip(x, y, contents) {
        $('<div id="tooltip">' + contents + '</div>').css({
            top: y - 16,
            left: x + 20
        }).appendTo('body').fadeIn();
    }

    var previousPoint = null;
    var textlines=" Градусов ";
    $('#graph-lines').bind('plothover', function (event, pos, item) {
        if (item) {
            if (previousPoint != item.dataIndex) {
                previousPoint = item.dataIndex;
                $('#tooltip').remove();
                var x = item.datapoint[0],
                y = item.datapoint[1];
                var temp = new Date(x);
                var dateStr = temp.getFullYear().toString() + "-" + 
                (1 + temp.getMonth()).toString() + "-" +
                temp.getDate().toString() + " " +
                temp.getHours().toString() + ":" +
                temp.getMinutes().toString() + ":" +
                temp.getSeconds().toString();
                showTooltip(item.pageX, item.pageY, y + textlines +  dateStr );
            }
        } else {
            $('#tooltip').remove();
            previousPoint = null;
        }
    });
    var textbars=' Градусов ';
    $('#graph-bars').bind('plothover', function (event, pos, item) {
        if (item) {
            if (previousPoint != item.dataIndex) {
                previousPoint = item.dataIndex;
                $('#tooltip').remove();
                var x = item.datapoint[0],
                y = item.datapoint[1];
                showTooltip(item.pageX, item.pageY, y + textbars + x );
            }
        } else {
            $('#tooltip').remove();
            previousPoint = null;
        }
    });
}





$(document).ready(function () {


    //repaint_graph(graphData);


});



// Graph Data ##############################################
    var graphData = [{
        // Visits
        data: [ [1354616367000, 0], [1354616372000, 1], [1354616377000, 7], [1354616381000, 8],
        [1354616870000, 13], [1354616875000, 23], [1354617842000, 32], [1354617846000, 33]
        ],
        color: '#71c73e',
        points: {
            radius: 5 
        //,fillColor: '#77b7c5'
        }
    }];