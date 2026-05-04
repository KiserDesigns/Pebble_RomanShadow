'use strict';

module.exports = {
  name: 'custom-minute',
  template: '<span class="component-select" style="display:inline-block;align-self:end;">\
  <label class="tap-highlight" style="padding:0.6rem 0.2rem 0.6rem 0;margin:0;">\
    <select data-manipulator-target {{each key: attributes}}{{key}}="{{this}}"{{/each}}>\
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
      <option value="24" class="item-select-option">24</option>\
      <option value="25" class="item-select-option">25</option>\
      <option value="26" class="item-select-option">26</option>\
      <option value="27" class="item-select-option">27</option>\
      <option value="28" class="item-select-option">28</option>\
      <option value="29" class="item-select-option">29</option>\
      <option value="30" class="item-select-option">30</option>\
      <option value="31" class="item-select-option">31</option>\
      <option value="32" class="item-select-option">32</option>\
      <option value="33" class="item-select-option">33</option>\
      <option value="34" class="item-select-option">34</option>\
      <option value="35" class="item-select-option">35</option>\
      <option value="36" class="item-select-option">36</option>\
      <option value="37" class="item-select-option">37</option>\
      <option value="38" class="item-select-option">38</option>\
      <option value="39" class="item-select-option">39</option>\
      <option value="40" class="item-select-option">40</option>\
      <option value="41" class="item-select-option">41</option>\
      <option value="42" class="item-select-option">42</option>\
      <option value="43" class="item-select-option">43</option>\
      <option value="44" class="item-select-option">44</option>\
      <option value="45" class="item-select-option">45</option>\
      <option value="46" class="item-select-option">46</option>\
      <option value="47" class="item-select-option">47</option>\
      <option value="48" class="item-select-option">48</option>\
      <option value="49" class="item-select-option">49</option>\
      <option value="50" class="item-select-option">50</option>\
      <option value="51" class="item-select-option">51</option>\
      <option value="52" class="item-select-option">52</option>\
      <option value="53" class="item-select-option">53</option>\
      <option value="54" class="item-select-option">54</option>\
      <option value="55" class="item-select-option">55</option>\
      <option value="56" class="item-select-option">56</option>\
      <option value="57" class="item-select-option">57</option>\
      <option value="58" class="item-select-option">58</option>\
      <option value="59" class="item-select-option">59</option>\
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