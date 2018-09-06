import React from 'react';
import PropTypes from 'prop-types';
import ReactSelect from 'react-select';
import 'react-select/dist/react-select.css';
import Input from '../../common/Input/Input/Input';
import Label from '../../common/Label/Label';
import Description from '../../common/Description/Description';
import Row from '../../common/Row/Row';
import styles from './MultiInputs.scss';

const MultiInputs = (props) => {
  const { options, onChange, presentationLabel, inputTypes, disabled, description } = props;
  return (
    <div className={styles.multiInputs}>
      <Label size='medium'>{presentationLabel}</Label>
      <Row>
        <Description>{description}</Description>
      </Row>
      <Row>
        {inputTypes.map((type) => (
          <Input
            key={type}
            label={type}
            placeholder={type}
            value={options[type] !== undefined ? Math.round(options[type]) : 100}
            disabled={disabled}
            onChange={onChange}
          />
        ))}
      </Row>
    </div>
  );
};

MultiInputs.propTypes = {
  options: PropTypes.object.isRequired,
  presentationLabel: PropTypes.string.isRequired,
  inputTypes: PropTypes.arrayOf(PropTypes.string),
  onChange: PropTypes.func,
}

MultiInputs.defaultProps = {
  options: { x: 0, y: 0, z: 0 },
  label: 'Loading',
  onChange: () => { },
  inputTypes: ['x', 'y', 'z']
}

export default MultiInputs;