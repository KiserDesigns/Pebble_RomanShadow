'use strict';

module.exports = {
  name: 'custom-toggle',
  template: '<span class="component-toggle" style="display:inline-block;">\
              <label class="tap-highlight"">\
                  <input\
                    data-manipulator-target\
                    type="checkbox"\
                    {{each key: attributes}}{{key}}="{{this}}"{{/each}}\
                  />\
                  <span class="graphic">\
                    <span class="slide"></span>\
                    <span class="marker"></span>\
                  </span>\
              </label>\
            </span>',
  style: '',
  manipulator: 'checked',
  defaults: {
    label: '',
    description: '',
    attributes: {}
  }
};
