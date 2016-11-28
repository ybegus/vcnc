module.exports = {
  // So parent files don't get applied
  root: true,
  env: {
    es6: true,
    browser: true,
    node: true,
  },
  extends: 'airbnb',
  parser: 'babel-eslint',
  parserOptions: {
    ecmaVersion: 7,
    sourceType: 'module',
    ecmaFeatures: {
      jsx: true,
      experimentalObjectRestSpread: true,
    }
  },
  plugins: [
    'babel',
    'react',
    'mocha',
    'material-ui',
  ],
  rules: {
    // React
    'react/display-name': 'error',
    'react/jsx-boolean-value': ['error', 'always'],
    'react/jsx-closing-bracket-location': 'error',
    'react/jsx-curly-spacing': 'error',
    'react/jsx-equals-spacing': 'error',
    'react/jsx-first-prop-new-line': ['error', 'multiline'],
    'react/jsx-handler-names': 'error',
    'react/jsx-indent-props': ['error', 2],
    'react/jsx-max-props-per-line': ['error', {maximum: 3}],
    'react/jsx-no-duplicate-props': 'error',
    'react/jsx-no-undef': 'error',
    'react/jsx-pascal-case': 'error',
    'react/jsx-space-before-closing': 'error',
    'react/jsx-uses-react': 'error',
    'react/jsx-uses-vars': 'error',
    'react/no-danger': 'error',
    'react/no-did-mount-set-state': 'error',
    'react/no-did-update-set-state': 'error',
    'react/no-direct-mutation-state': 'error',
    'react/no-unknown-property': 'error',
    'react/no-is-mounted': 'error',
    'react/prefer-es6-class': 'error',
    'react/prop-types': 'error',
    'react/react-in-jsx-scope': 'error',
    'react/require-extension': 'error',
    'react/self-closing-comp': 'error',
    'react/sort-comp': 'error',
    'react/sort-prop-types': 'error',
    'react/wrap-multilines': 'error',
    'react/jsx-indent': ['error', 2],

    // React Disabled
    'react/jsx-no-bind': 'off',
    'react/jsx-no-literals': 'off',
    'react/jsx-sort-props': 'off',
    'react/no-set-state': 'warn',

    // Material-UI
    'material-ui/docgen-ignore-before-comment': 'error',

    'mocha/handle-done-callback': 'error',
    'mocha/no-exclusive-tests': 'error',
    'mocha/no-global-tests': 'error',
    'mocha/no-pending-tests': 'error',
    'mocha/no-skipped-tests': 'error',
  },
};
