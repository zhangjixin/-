Page({
  onShow: function() {
    wx.setNavigationBarTitle({
      title: 'C606智能工作计时器'
    })
        this.setData({
          workTime: wx.getStorageSync('workTime'),

        })
      

  },
  changeWorkTime: function(e) {
  	wx.setStorage({
  		key: 'workTime',
  		data: e.detail.value
  	})
  },


  /**
   * 页面的初始数据
   */
  data: {
    ah: "7",
    am: "30",
    status: "ON",
    lx: "3",
    lox: "568",
    up: 23,
    upm: 0,
  
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    //加载时就获取后台的数据
    this.get_data()
  },
  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  },
  send4: function () {
    this.data.status = ((this.data.status == "ON") ? "OFF" : "ON"),
      this.setData({
        status: this.data.status
      })
    wx.showToast({
      title: this.data.status,
      duration: 1000
    })
    this.sendRequset(this.makeObj(this.data.ah, this.data.am, this.data.status, this.data.lx, this.data.up, this.data.upm, ""));
  },

  send6: function () {
    this.data.lx ? this.data.lx = 0 : this.data.lx = 10
    this.setData({
      tem: this.data.tem
    })

    this.sendRequset(this.makeObj(this.data.ah, this.data.am, this.data.status, this.data.lx, this.data.up, this.data.upm, "")),
      wx.showToast({
        title: "Loading",
        duration: 1000
      })

    // this.get_data()
  },

  send5: function () {

    this.setData({
      tem: this.data.tem
    })
    wx.showToast({
      title: "SEND SUCCESS",
      duration: 1000
    })
    this.sendRequset(this.makeObj(this.data.ah, this.data.am, this.data.status, this.data.lx, this.data.up, this.data.upm, ""));
  },
  //Function of the function is to auto move slide  ,every second ++
  change1: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        ah: e.detail.value,

      })
    }

  },
  change2: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        am: e.detail.value,

      })
    }

  },
  change3: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        tem: e.detail.value,

      })
    }

  },
  change4: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        up: e.detail.value,

      })
    }

  },
  change5: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        upm: e.detail.value,

      })
    }

  },
  sendRequset: function (obj) {
    wx.request(obj);
  },
  makeObj: function (h, m, sta, t, msg) {
    var obj = {
      url: "http://api.heclouds.com/devices/503230484/datapoints?type=3",

      header: {
        "Content-Type": "application/json",
        "api-key": "ezz1czmZvq8ByuuF4Fu9uHKvYL0= ",
      },
      method: "post",
      data: {

        "ah": h,
        "am": m,
        "status": sta == "ON" ? 1 : 2,
        "tem": t,
        //"lox":lox,

      },
      success: function (res) {
        if (msg != "") {
          wx.showToast({
            title: msg,
            duration: 2000
          })

        }
      }
    }
    return obj;
  },

  get_data: function () {
    // 获取数据
    var that = this;
    wx.request({
      url: 'https://api.heclouds.com/devices/503230484/datapoints?datastream_id=ah,am,status,tem&limit=1',
      header: {
        'content-type': 'application/json',
        'api-key': 'ezz1czmZvq8ByuuF4Fu9uHKvYL0= '
      },

      success: function (res) {
        console.log(res.data)
        wx.showToast({
          title: "OK",
          duration: 1000
        })



        console.log(res.data.data.datastreams[0].datapoints[0].value),
          console.log(res.data.data.datastreams[1].datapoints[0].value),
          console.log(res.data.data.datastreams[2].datapoints[0].value),
          console.log(res.data.data.datastreams[3].datapoints[0].value),
          console.log(res.data.data.datastreams[4].datapoints[0].value),
          console.log(res.data.data.datastreams[5].datapoints[0].value),
          that.setData({
            ah: res.data.data.datastreams[1].datapoints[0].value,
            am: res.data.data.datastreams[3].datapoints[0].value,
            tem: res.data.data.datastreams[4].datapoints[0].value,


             up: res.data.data.datastreams[2].datapoints[0].value,
              upm: res.data.data.datastreams[0].datapoints[0].value,
            status: ((res.data.data.datastreams[5].datapoints[0].value) == 1 ? "ON" : "OFF"),
          })


      }
    })
  },

})