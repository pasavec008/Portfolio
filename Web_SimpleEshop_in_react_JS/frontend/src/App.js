import React from 'react';
import './App.css';
import Navigator from './components/Navigator';
import Admin from './components/Admin';
import {
  BrowserRouter as Router,
  Routes,
  Route
} from 'react-router-dom';

class App extends React.Component {
  render(){
    return (
      <div className="App">
        <Router>
          <Routes>
            <Route path='' element={<Navigator />} />
            <Route path='/admin' element={<Admin />} />
          </Routes>
        </Router>
      </div>
    );
  }
}

export default App;
