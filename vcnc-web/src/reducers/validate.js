export const validateNonNegativeInteger = (int, name, errors) => {
  if (/^[0-9]+$/.test(int)) {
    if (errors && errors[name]) {
      const e = { ...errors };
      delete e[name];
      return e;
    }
    return errors;
  }
  const e = { ...errors };
  e[name] = ' ';
  return e;
};

export const validatePath = (path, name, errors) => {
  let error;
  if (!path) {
    error = 'Cannot be empty';
  } else if (path.charAt(0) !== '/') {
    error = 'Must begin with /';
  } else if (errors && errors[name]) {
    const e = { ...errors };
    delete e[name];
    return e;
  } else {
    return errors || {};
  }
  const e = { ...errors };
  e[name] = error;
  return e;
};
