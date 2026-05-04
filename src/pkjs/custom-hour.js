'use strict';

module.exports = {
  name: 'custom-hour',
  template: '<span class="component-select" style="display:inline-block;align-self:end;">\
  <label class="tap-highlight" style="padding:0.6rem 0 0.6rem 0.2rem;margin:0">\
    <select data-manipulator-target="">\
      <option value="00" class="item-select-option">00</option>\
      <option value="01" class="item-select-option">01</option>\
      <option value="02" class="item-select-option">02</option>\
      <option value="03" class="item-select-option">03</option>\
      <option value="04" class="item-select-option">04</option>\
      <option value="05" class="item-select-option">05</option>\
      <option value="06" class="item-select-option">06</option>\
      <option value="07" class="item-select-option">07</option>\
      <option value="08" class="item-select-option">08</option>\
      <option value="09" class="item-select-option">09</option>\
      <option value="10" class="item-select-option">10</option>\
      <option value="11" class="item-select-option">11</option>\
      <option value="12" class="item-select-option">12</option>\
      <option value="13" class="item-select-option">13</option>\
      <option value="14" class="item-select-option">14</option>\
      <option value="15" class="item-select-option">15</option>\
      <option value="16" class="item-select-option">16</option>\
      <option value="17" class="item-select-option">17</option>\
      <option value="18" class="item-select-option">18</option>\
      <option value="19" class="item-select-option">19</option>\
      <option value="20" class="item-select-option">20</option>\
      <option value="21" class="item-select-option">21</option>\
      <option value="22" class="item-select-option">22</option>\
      <option value="23" class="item-select-option">23</option>\
    </select>\
  </label>\
</span>',
  style: "",
  manipulator: 'val',
  defaults: {
    label: '',
    options: [],
    description: '',
    attributes: {}
  },
  initialize: function() {
 /**   var self = this;

    var $value = self.$element.select('.value');

    /**
     * Updates the HTML value of the component to match the slected option's label
     * @return {void}
     */
 /**   function setValueDisplay() {
      var selectedIndex = self.$manipulatorTarget.get('selectedIndex');
      var $options = self.$manipulatorTarget.select('option');
      var value = $options[selectedIndex] && $options[selectedIndex].innerHTML;
      $value.set('innerHTML', value);
    }

    setValueDisplay();
    self.on('change', setValueDisplay);
    **/
  }
};