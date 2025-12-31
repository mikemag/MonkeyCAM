import React from 'react';
import { useLocation, useNavigate, useParams } from 'react-router-dom';

const withRouter = WrappedComponent => {
  function ComponentWithRouterProp(props) {
    const location = useLocation();
    const navigate = useNavigate();
    const params = useParams();
    return (
      <WrappedComponent
        {...props}
        location={location}
        navigate={navigate}
        params={params}
      />
    );
  }

  const wrappedName = WrappedComponent.displayName || WrappedComponent.name;
  ComponentWithRouterProp.displayName = `withRouter(${wrappedName})`;

  return ComponentWithRouterProp;
};

export default withRouter;
